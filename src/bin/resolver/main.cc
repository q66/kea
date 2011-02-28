// Copyright (C) 2009  Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>

#include <string>
#include <iostream>

#include <boost/foreach.hpp>

#include <asiolink/asiolink.h>

#include <exceptions/exceptions.h>

#include <dns/buffer.h>
#include <dns/message.h>
#include <dns/messagerenderer.h>

#include <cc/session.h>
#include <cc/data.h>
#include <config/ccsession.h>

#include <xfr/xfrout_client.h>

#include <auth/change_user.h>
#include <auth/common.h>

#include <resolver/spec_config.h>
#include <resolver/resolver.h>

#include <cache/resolver_cache.h>
#include <nsas/nameserver_address_store.h>

#include <log/dummylog.h>

using namespace std;
using namespace isc::cc;
using namespace isc::config;
using namespace isc::data;
using isc::log::dlog;
using namespace asiolink;

namespace {

// Default port current 5300 for testing purposes
static const string PROGRAM = "Resolver";

IOService io_service;
static boost::shared_ptr<Resolver> resolver;

ConstElementPtr
my_config_handler(ConstElementPtr new_config) {
    return (resolver->updateConfig(new_config));
}

ConstElementPtr
my_command_handler(const string& command, ConstElementPtr args) {
    ConstElementPtr answer = createAnswer();

    if (command == "print_message") {
        cout << args << endl;
        /* let's add that message to our answer as well */
        answer = createAnswer(0, args);
    } else if (command == "shutdown") {
        io_service.stop();
    }

    return (answer);
}

void
usage() {
    cerr << "Usage:  b10-resolver [-u user] [-v]" << endl;
    cerr << "\t-u: change process UID to the specified user" << endl;
    cerr << "\t-v: verbose output" << endl;
    exit(1);
}
} // end of anonymous namespace

int
main(int argc, char* argv[]) {
    isc::log::dprefix = "b10-resolver";
    int ch;
    const char* uid = NULL;

    while ((ch = getopt(argc, argv, "u:v")) != -1) {
        switch (ch) {
        case 'u':
            uid = optarg;
            break;
        case 'v':
            isc::log::denabled = true;
            break;
        case '?':
        default:
            usage();
        }
    }

    if (argc - optind > 0) {
        usage();
    }

    if (isc::log::denabled) { // Show the command line
        string cmdline("Command line:");
        for (int i = 0; i < argc; ++ i) {
            cmdline = cmdline + " " + argv[i];
        }
        dlog(cmdline);
    }

    int ret = 0;

    Session* cc_session = NULL;
    ModuleCCSession* config_session = NULL;
    //try {
        string specfile;
        if (getenv("B10_FROM_BUILD")) {
            specfile = string(getenv("B10_FROM_BUILD")) +
                "/src/bin/resolver/resolver.spec";
        } else {
            specfile = string(RESOLVER_SPECFILE_LOCATION);
        }

        resolver = boost::shared_ptr<Resolver>(new Resolver());
        dlog("Server created.");

        SimpleCallback* checkin = resolver->getCheckinProvider();
        DNSLookup* lookup = resolver->getDNSLookupProvider();
        DNSAnswer* answer = resolver->getDNSAnswerProvider();

        isc::nsas::NameserverAddressStore nsas(resolver);
        resolver->setNameserverAddressStore(nsas);

        isc::cache::ResolverCache cache;
        resolver->setCache(cache);
        
        // TODO priming query, remove root from direct
        // Fake a priming query result here (TODO2 how to flag non-expiry?)
        // propagation to runningquery. And check for forwarder mode?
        isc::dns::QuestionPtr root_question(new isc::dns::Question(
                                            isc::dns::Name("."),
                                            isc::dns::RRClass::IN(),
                                            isc::dns::RRType::NS()));
        isc::dns::RRsetPtr root_ns_rrset(new isc::dns::RRset(isc::dns::Name("."), 
                                         isc::dns::RRClass::IN(),
                                         isc::dns::RRType::NS(),
                                         isc::dns::RRTTL(8888)));
        root_ns_rrset->addRdata(isc::dns::rdata::createRdata(isc::dns::RRType::NS(),
                                                             isc::dns::RRClass::IN(),
                                                             "l.root-servers.net."));
        isc::dns::RRsetPtr root_a_rrset(new isc::dns::RRset(isc::dns::Name("l.root-servers.net"), 
                                        isc::dns::RRClass::IN(),
                                        isc::dns::RRType::A(),
                                        isc::dns::RRTTL(8888)));
        root_a_rrset->addRdata(isc::dns::rdata::createRdata(isc::dns::RRType::A(),
                                                             isc::dns::RRClass::IN(),
                                                             "199.7.83.42"));
        isc::dns::RRsetPtr root_aaaa_rrset(new isc::dns::RRset(isc::dns::Name("l.root-servers.net"), 
                                        isc::dns::RRClass::IN(),
                                        isc::dns::RRType::AAAA(),
                                        isc::dns::RRTTL(8888)));
        root_aaaa_rrset->addRdata(isc::dns::rdata::createRdata(isc::dns::RRType::AAAA(),
                                                             isc::dns::RRClass::IN(),
                                                             "2001:500:3::42"));
        isc::dns::MessagePtr priming_result(new isc::dns::Message(isc::dns::Message::RENDER));
        priming_result->addQuestion(root_question);
        priming_result->addRRset(isc::dns::Message::SECTION_ANSWER, root_ns_rrset);
        priming_result->addRRset(isc::dns::Message::SECTION_ADDITIONAL, root_a_rrset);
        priming_result->addRRset(isc::dns::Message::SECTION_ADDITIONAL, root_aaaa_rrset);
        cache.update(*priming_result);
        cache.update(root_ns_rrset);
        cache.update(root_a_rrset);
        cache.update(root_aaaa_rrset);
        
        DNSService dns_service(io_service, checkin, lookup, answer);
        resolver->setDNSService(dns_service);
        dlog("IOService created.");

        cc_session = new Session(io_service.get_io_service());
        dlog("Configuration session channel created.");

        config_session = new ModuleCCSession(specfile, *cc_session,
                                             my_config_handler,
                                             my_command_handler);
        dlog("Configuration channel established.");

        // FIXME: This does not belong here, but inside Boss
        if (uid != NULL) {
            changeUser(uid);
        }

        resolver->setConfigSession(config_session);
        dlog("Config loaded");

        dlog("Server started.");
        io_service.run();
    //} catch (const std::exception& ex) {
    //    dlog(string("Server failed: ") + ex.what(),true);
    //    ret = 1;
    //}

    delete config_session;
    delete cc_session;

    return (ret);
}
