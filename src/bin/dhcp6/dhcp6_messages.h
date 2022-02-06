// File created from ../../../src/bin/dhcp6/dhcp6_messages.mes

#ifndef DHCP6_MESSAGES_H
#define DHCP6_MESSAGES_H

#include <log/message_types.h>

namespace isc {
namespace dhcp {

extern const isc::log::MessageID DHCP6_ACTIVATE_INTERFACE;
extern const isc::log::MessageID DHCP6_ADD_GLOBAL_STATUS_CODE;
extern const isc::log::MessageID DHCP6_ADD_STATUS_CODE_FOR_IA;
extern const isc::log::MessageID DHCP6_ALREADY_RUNNING;
extern const isc::log::MessageID DHCP6_BUFFER_RECEIVED;
extern const isc::log::MessageID DHCP6_BUFFER_UNPACK;
extern const isc::log::MessageID DHCP6_BUFFER_WAIT_SIGNAL;
extern const isc::log::MessageID DHCP6_CB_ON_DEMAND_FETCH_UPDATES_FAIL;
extern const isc::log::MessageID DHCP6_CB_PERIODIC_FETCH_UPDATES_FAIL;
extern const isc::log::MessageID DHCP6_CB_PERIODIC_FETCH_UPDATES_RETRIES_EXHAUSTED;
extern const isc::log::MessageID DHCP6_CLASS_ASSIGNED;
extern const isc::log::MessageID DHCP6_CLASS_UNCONFIGURED;
extern const isc::log::MessageID DHCP6_CLASS_UNDEFINED;
extern const isc::log::MessageID DHCP6_CLASS_UNTESTABLE;
extern const isc::log::MessageID DHCP6_COMMAND_RECEIVED;
extern const isc::log::MessageID DHCP6_CONFIG_COMPLETE;
extern const isc::log::MessageID DHCP6_CONFIG_LOAD_FAIL;
extern const isc::log::MessageID DHCP6_CONFIG_PACKET_QUEUE;
extern const isc::log::MessageID DHCP6_CONFIG_RECEIVED;
extern const isc::log::MessageID DHCP6_CONFIG_START;
extern const isc::log::MessageID DHCP6_CONFIG_SYNTAX_WARNING;
extern const isc::log::MessageID DHCP6_CONFIG_UNRECOVERABLE_ERROR;
extern const isc::log::MessageID DHCP6_CONFIG_UNSUPPORTED_OBJECT;
extern const isc::log::MessageID DHCP6_CONFIG_UPDATE;
extern const isc::log::MessageID DHCP6_DB_BACKEND_STARTED;
extern const isc::log::MessageID DHCP6_DB_RECONNECT_DISABLED;
extern const isc::log::MessageID DHCP6_DB_RECONNECT_FAILED;
extern const isc::log::MessageID DHCP6_DB_RECONNECT_LOST_CONNECTION;
extern const isc::log::MessageID DHCP6_DB_RECONNECT_NO_DB_CTL;
extern const isc::log::MessageID DHCP6_DB_RECONNECT_SUCCEEDED;
extern const isc::log::MessageID DHCP6_DDNS_CREATE_ADD_NAME_CHANGE_REQUEST;
extern const isc::log::MessageID DHCP6_DDNS_FQDN_GENERATED;
extern const isc::log::MessageID DHCP6_DDNS_GENERATED_FQDN_UPDATE_FAIL;
extern const isc::log::MessageID DHCP6_DDNS_GENERATE_FQDN;
extern const isc::log::MessageID DHCP6_DDNS_RECEIVE_FQDN;
extern const isc::log::MessageID DHCP6_DDNS_REMOVE_OLD_LEASE_FQDN;
extern const isc::log::MessageID DHCP6_DDNS_REQUEST_SEND_FAILED;
extern const isc::log::MessageID DHCP6_DDNS_RESPONSE_FQDN_DATA;
extern const isc::log::MessageID DHCP6_DDNS_SEND_FQDN;
extern const isc::log::MessageID DHCP6_DEACTIVATE_INTERFACE;
extern const isc::log::MessageID DHCP6_DECLINE_FAIL;
extern const isc::log::MessageID DHCP6_DECLINE_FAIL_DUID_MISMATCH;
extern const isc::log::MessageID DHCP6_DECLINE_FAIL_IAID_MISMATCH;
extern const isc::log::MessageID DHCP6_DECLINE_FAIL_LEASE_WITHOUT_DUID;
extern const isc::log::MessageID DHCP6_DECLINE_FAIL_NO_LEASE;
extern const isc::log::MessageID DHCP6_DECLINE_LEASE;
extern const isc::log::MessageID DHCP6_DECLINE_PROCESS_IA;
extern const isc::log::MessageID DHCP6_DEVELOPMENT_VERSION;
extern const isc::log::MessageID DHCP6_DHCP4O6_PACKET_RECEIVED;
extern const isc::log::MessageID DHCP6_DHCP4O6_RECEIVE_FAIL;
extern const isc::log::MessageID DHCP6_DHCP4O6_RECEIVING;
extern const isc::log::MessageID DHCP6_DHCP4O6_SEND_FAIL;
extern const isc::log::MessageID DHCP6_DYNAMIC_RECONFIGURATION;
extern const isc::log::MessageID DHCP6_DYNAMIC_RECONFIGURATION_FAIL;
extern const isc::log::MessageID DHCP6_DYNAMIC_RECONFIGURATION_SUCCESS;
extern const isc::log::MessageID DHCP6_FLEX_ID;
extern const isc::log::MessageID DHCP6_HANDLE_SIGNAL_EXCEPTION;
extern const isc::log::MessageID DHCP6_HOOKS_LIBS_RELOAD_FAIL;
extern const isc::log::MessageID DHCP6_HOOK_BUFFER_RCVD_DROP;
extern const isc::log::MessageID DHCP6_HOOK_BUFFER_RCVD_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_BUFFER_SEND_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_DECLINE_DROP;
extern const isc::log::MessageID DHCP6_HOOK_DECLINE_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_LEASE6_RELEASE_NA_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_LEASE6_RELEASE_PD_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_LEASES6_COMMITTED_DROP;
extern const isc::log::MessageID DHCP6_HOOK_LEASES6_COMMITTED_PARK;
extern const isc::log::MessageID DHCP6_HOOK_LEASES6_PARKING_LOT_FULL;
extern const isc::log::MessageID DHCP6_HOOK_PACKET_RCVD_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_PACKET_SEND_DROP;
extern const isc::log::MessageID DHCP6_HOOK_PACKET_SEND_SKIP;
extern const isc::log::MessageID DHCP6_HOOK_SUBNET6_SELECT_DROP;
extern const isc::log::MessageID DHCP6_HOOK_SUBNET6_SELECT_SKIP;
extern const isc::log::MessageID DHCP6_INIT_FAIL;
extern const isc::log::MessageID DHCP6_LEASE_ADVERT;
extern const isc::log::MessageID DHCP6_LEASE_ADVERT_FAIL;
extern const isc::log::MessageID DHCP6_LEASE_ALLOC;
extern const isc::log::MessageID DHCP6_LEASE_ALLOC_FAIL;
extern const isc::log::MessageID DHCP6_LEASE_DATA;
extern const isc::log::MessageID DHCP6_LEASE_NA_WITHOUT_DUID;
extern const isc::log::MessageID DHCP6_LEASE_PD_WITHOUT_DUID;
extern const isc::log::MessageID DHCP6_LEASE_RENEW;
extern const isc::log::MessageID DHCP6_LEASE_REUSE;
extern const isc::log::MessageID DHCP6_MULTI_THREADING_INFO;
extern const isc::log::MessageID DHCP6_NOT_RUNNING;
extern const isc::log::MessageID DHCP6_NO_INTERFACES;
extern const isc::log::MessageID DHCP6_NO_SOCKETS_OPEN;
extern const isc::log::MessageID DHCP6_OPEN_SOCKET;
extern const isc::log::MessageID DHCP6_OPEN_SOCKET_FAIL;
extern const isc::log::MessageID DHCP6_PACKET_DROP_DHCP_DISABLED;
extern const isc::log::MessageID DHCP6_PACKET_DROP_DROP_CLASS;
extern const isc::log::MessageID DHCP6_PACKET_DROP_DROP_CLASS2;
extern const isc::log::MessageID DHCP6_PACKET_DROP_DROP_CLASS_EARLY;
extern const isc::log::MessageID DHCP6_PACKET_DROP_DUPLICATE;
extern const isc::log::MessageID DHCP6_PACKET_DROP_PARSE_FAIL;
extern const isc::log::MessageID DHCP6_PACKET_DROP_SERVERID_MISMATCH;
extern const isc::log::MessageID DHCP6_PACKET_DROP_UNICAST;
extern const isc::log::MessageID DHCP6_PACKET_OPTIONS_SKIPPED;
extern const isc::log::MessageID DHCP6_PACKET_PROCESS_EXCEPTION;
extern const isc::log::MessageID DHCP6_PACKET_PROCESS_FAIL;
extern const isc::log::MessageID DHCP6_PACKET_PROCESS_STD_EXCEPTION;
extern const isc::log::MessageID DHCP6_PACKET_QUEUE_FULL;
extern const isc::log::MessageID DHCP6_PACKET_RECEIVED;
extern const isc::log::MessageID DHCP6_PACKET_RECEIVE_FAIL;
extern const isc::log::MessageID DHCP6_PACKET_SEND;
extern const isc::log::MessageID DHCP6_PACKET_SEND_FAIL;
extern const isc::log::MessageID DHCP6_PACK_FAIL;
extern const isc::log::MessageID DHCP6_PARSER_COMMIT_EXCEPTION;
extern const isc::log::MessageID DHCP6_PARSER_COMMIT_FAIL;
extern const isc::log::MessageID DHCP6_PARSER_EXCEPTION;
extern const isc::log::MessageID DHCP6_PARSER_FAIL;
extern const isc::log::MessageID DHCP6_PD_LEASE_ADVERT;
extern const isc::log::MessageID DHCP6_PD_LEASE_ADVERT_FAIL;
extern const isc::log::MessageID DHCP6_PD_LEASE_ALLOC;
extern const isc::log::MessageID DHCP6_PD_LEASE_ALLOC_FAIL;
extern const isc::log::MessageID DHCP6_PD_LEASE_RENEW;
extern const isc::log::MessageID DHCP6_PD_LEASE_REUSE;
extern const isc::log::MessageID DHCP6_PROCESS_IA_NA_EXTEND;
extern const isc::log::MessageID DHCP6_PROCESS_IA_NA_RELEASE;
extern const isc::log::MessageID DHCP6_PROCESS_IA_NA_REQUEST;
extern const isc::log::MessageID DHCP6_PROCESS_IA_PD_EXTEND;
extern const isc::log::MessageID DHCP6_PROCESS_IA_PD_REQUEST;
extern const isc::log::MessageID DHCP6_QUERY_DATA;
extern const isc::log::MessageID DHCP6_RAPID_COMMIT;
extern const isc::log::MessageID DHCP6_RECLAIM_EXPIRED_LEASES_FAIL;
extern const isc::log::MessageID DHCP6_RELEASE_NA;
extern const isc::log::MessageID DHCP6_RELEASE_NA_FAIL;
extern const isc::log::MessageID DHCP6_RELEASE_NA_FAIL_WRONG_DUID;
extern const isc::log::MessageID DHCP6_RELEASE_NA_FAIL_WRONG_IAID;
extern const isc::log::MessageID DHCP6_RELEASE_PD;
extern const isc::log::MessageID DHCP6_RELEASE_PD_FAIL;
extern const isc::log::MessageID DHCP6_RELEASE_PD_FAIL_WRONG_DUID;
extern const isc::log::MessageID DHCP6_RELEASE_PD_FAIL_WRONG_IAID;
extern const isc::log::MessageID DHCP6_REQUIRED_OPTIONS_CHECK_FAIL;
extern const isc::log::MessageID DHCP6_RESERVATIONS_LOOKUP_FIRST_ENABLED;
extern const isc::log::MessageID DHCP6_RESPONSE_DATA;
extern const isc::log::MessageID DHCP6_SERVER_FAILED;
extern const isc::log::MessageID DHCP6_SHUTDOWN;
extern const isc::log::MessageID DHCP6_SHUTDOWN_REQUEST;
extern const isc::log::MessageID DHCP6_SOCKET_UNICAST;
extern const isc::log::MessageID DHCP6_SRV_CONSTRUCT_ERROR;
extern const isc::log::MessageID DHCP6_SRV_D2STOP_ERROR;
extern const isc::log::MessageID DHCP6_SRV_UNLOAD_LIBRARIES_ERROR;
extern const isc::log::MessageID DHCP6_STANDALONE;
extern const isc::log::MessageID DHCP6_STARTED;
extern const isc::log::MessageID DHCP6_STARTING;
extern const isc::log::MessageID DHCP6_START_INFO;
extern const isc::log::MessageID DHCP6_SUBNET_DATA;
extern const isc::log::MessageID DHCP6_SUBNET_DYNAMICALLY_CHANGED;
extern const isc::log::MessageID DHCP6_SUBNET_SELECTED;
extern const isc::log::MessageID DHCP6_SUBNET_SELECTION_FAILED;
extern const isc::log::MessageID DHCP6_UNKNOWN_MSG_RECEIVED;
extern const isc::log::MessageID DHCP6_USING_SERVERID;

} // namespace dhcp
} // namespace isc

#endif // DHCP6_MESSAGES_H
