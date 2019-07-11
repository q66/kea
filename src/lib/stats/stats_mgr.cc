// Copyright (C) 2015,2017 Internet Systems Consortium, Inc. ("ISC")
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <config.h>

#include <exceptions/exceptions.h>
#include <stats/stats_mgr.h>
#include <cc/data.h>
#include <cc/command_interpreter.h>
#include <util/boost_time_utils.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace isc::data;
using namespace isc::config;

namespace isc {
namespace stats {

StatsMgr& StatsMgr::instance() {
    static StatsMgr stats_mgr;
    return (stats_mgr);
}

StatsMgr::StatsMgr() :
    global_(new StatContext()) {

}

void StatsMgr::setValue(const std::string& name, const int64_t value) {
    setValueInternal(name, value);
}

void StatsMgr::setValue(const std::string& name, const double value) {
    setValueInternal(name, value);
}

void StatsMgr::setValue(const std::string& name, const StatsDuration& value) {
    setValueInternal(name, value);
}
void StatsMgr::setValue(const std::string& name, const std::string& value) {
    setValueInternal(name, value);
}

void StatsMgr::addValue(const std::string& name, const int64_t value) {
    addValueInternal(name, value);
}

void StatsMgr::addValue(const std::string& name, const double value) {
    addValueInternal(name, value);
}

void StatsMgr::addValue(const std::string& name, const StatsDuration& value) {
    addValueInternal(name, value);
}

void StatsMgr::addValue(const std::string& name, const std::string& value) {
    addValueInternal(name, value);
}

ObservationPtr StatsMgr::getObservation(const std::string& name) const {
    /// @todo: Implement contexts.
    // Currently we keep everything in a global context.
    return (global_->get(name));
}

void StatsMgr::addObservation(const ObservationPtr& stat) {
    /// @todo: Implement contexts.
    // Currently we keep everything in a global context.
    return (global_->add(stat));
}

bool StatsMgr::deleteObservation(const std::string& name) {
    /// @todo: Implement contexts.
    // Currently we keep everything in a global context.
    return (global_->del(name));
}

bool StatsMgr::setMaxSampleAge(const std::string& name,
                               const StatsDuration& duration) {
    ObservationPtr obs = getObservation(name);
    if (obs) {
        obs->setMaxSampleAge(duration);
        return (true);
    } else {
        return (false);
    }
}

bool StatsMgr::setMaxSampleCount(const std::string& name, uint32_t max_samples) {
    ObservationPtr obs = getObservation(name);
    if (obs) {
        obs->setMaxSampleCount(max_samples);
        return (true);
    } else {
        return (false);
    }
}

bool StatsMgr::reset(const std::string& name) {
    ObservationPtr obs = getObservation(name);
    if (obs) {
        obs->reset();
        return (true);
    } else {
        return (false);
    }
}

bool StatsMgr::del(const std::string& name) {
    return (global_->del(name));
}

void StatsMgr::removeAll() {
    global_->stats_.clear();
}

isc::data::ConstElementPtr StatsMgr::get(const std::string& name) const {
    isc::data::ElementPtr response = isc::data::Element::createMap(); // a map
    ObservationPtr obs = getObservation(name);
    if (obs) {
        response->set(name, obs->getJSON()); // that contains observations
    }
    return (response);
}

isc::data::ConstElementPtr StatsMgr::getAll() const {
    isc::data::ElementPtr map = isc::data::Element::createMap(); // a map

    // Let's iterate over all stored statistics...
    for (std::map<std::string, ObservationPtr>::iterator s = global_->stats_.begin();
         s != global_->stats_.end(); ++s) {

        // ... and add each of them to the map.
        map->set(s->first, s->second->getJSON());
    }
    return (map);
}

void StatsMgr::resetAll() {
    // Let's iterate over all stored statistics...
    for (std::map<std::string, ObservationPtr>::iterator s = global_->stats_.begin();
         s != global_->stats_.end(); ++s) {

        // ... and reset each statistic.
        s->second->reset();
    }
}

size_t StatsMgr::getSize(const std::string& name) const {
    ObservationPtr obs = getObservation(name);
    size_t size = 0;
    if (obs) {
        size = obs->getSize();
    }
    return (size);
}

size_t StatsMgr::count() const {
    return (global_->stats_.size());
}

isc::data::ConstElementPtr
StatsMgr::statisticGetHandler(const std::string& /*name*/,
                              const isc::data::ConstElementPtr& params) {
    std::string name, error;
    if (!getStatName(params, name, error)) {
        return (createAnswer(CONTROL_RESULT_ERROR, error));
    }
    return (createAnswer(CONTROL_RESULT_SUCCESS,
                         instance().get(name)));
}

isc::data::ConstElementPtr
StatsMgr::statisticResetHandler(const std::string& /*name*/,
                                const isc::data::ConstElementPtr& params) {
    std::string name, error;
    if (!getStatName(params, name, error)) {
        return (createAnswer(CONTROL_RESULT_ERROR, error));
    }

    if (instance().reset(name)) {
        return (createAnswer(CONTROL_RESULT_SUCCESS,
                             "Statistic '" + name + "' reset."));
    } else {
        return (createAnswer(CONTROL_RESULT_ERROR,
                             "No '" + name + "' statistic found"));
    }
}

isc::data::ConstElementPtr
StatsMgr::statisticRemoveHandler(const std::string& /*name*/,
                                 const isc::data::ConstElementPtr& params) {
    std::string name, error;
    if (!getStatName(params, name, error)) {
        return (createAnswer(CONTROL_RESULT_ERROR, error));
    }
    if (instance().del(name)) {
        return (createAnswer(CONTROL_RESULT_SUCCESS,
                             "Statistic '" + name + "' removed."));
    } else {
        return (createAnswer(CONTROL_RESULT_ERROR,
                             "No '" + name + "' statistic found"));
    }

}

isc::data::ConstElementPtr
StatsMgr::statisticRemoveAllHandler(const std::string& /*name*/,
                                    const isc::data::ConstElementPtr& /*params*/) {
    instance().removeAll();
    return (createAnswer(CONTROL_RESULT_SUCCESS,
                         "All statistics removed."));
}

isc::data::ConstElementPtr
StatsMgr::statisticGetAllHandler(const std::string& /*name*/,
                                 const isc::data::ConstElementPtr& /*params*/) {
    ConstElementPtr all_stats = instance().getAll();
    return (createAnswer(CONTROL_RESULT_SUCCESS, all_stats));
}

isc::data::ConstElementPtr
StatsMgr::statisticResetAllHandler(const std::string& /*name*/,
                                   const isc::data::ConstElementPtr& /*params*/) {
    instance().resetAll();
    return (createAnswer(CONTROL_RESULT_SUCCESS,
                         "All statistics reset to neutral values."));
}

bool
StatsMgr::getStatName(const isc::data::ConstElementPtr& params,
                      std::string& name,
                      std::string& reason) {
    if (!params) {
        reason = "Missing mandatory 'name' parameter.";
        return (false);
    }
    ConstElementPtr stat_name = params->get("name");
    if (!stat_name) {
        reason = "Missing mandatory 'name' parameter.";
        return (false);
    }
    if (stat_name->getType() != Element::string) {
        reason = "'name' parameter expected to be a string.";
        return (false);
    }

    name = stat_name->stringValue();
    return (true);
}

};
};
