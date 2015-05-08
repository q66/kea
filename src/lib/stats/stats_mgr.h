// Copyright (C) 2015 Internet Systems Consortium, Inc. ("ISC")
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

#ifndef STATSMGR_H
#define STATSMGR_H

#include <stats/observation.h>
#include <stats/context.h>
#include <boost/noncopyable.hpp>

#include <map>
#include <string>
#include <vector>

namespace isc {
namespace stats {

class StatsMgr : public boost::noncopyable {
 public:

    /// @brief Statistics Manager accessor method.
    static StatsMgr& instance();

    // methods used data producers
    void addValue(const std::string& name, uint64_t value);
    void addValue(const std::string& name, double value);
    void addValue(const std::string& name, StatsDuration time);
    void addValue(const std::string& name, const std::string& value);
    void setValue(const std::string& name, uint64_t value);
    void setValue(const std::string& name, double value);
    void setValue(const std::string& name, StatsDuration value);
    void setValue(const std::string& name, const std::string& value);

    /// @brief determines whether a given statistic is kept as a single value
    ///        or as a number of values
    /// 
    /// Specifies that statistic name should be stored not as a single value,
    /// but rather as a set of values. duration determines the timespan.
    /// Samples older than duration will be discarded. This is time-constrained
    /// approach. For sample count constrained approach, see @ref
    /// setMaxSampleCount() below.
    ///
    /// @todo: Not implemented.
    ///
    /// Example: to set a statistic to keep observations for the last 5 minutes,
    /// call setMaxSampleAge("incoming-packets", time_duration(0,5,0,0));
    /// to revert statistic to a single value, call:
    /// setMaxSampleAge("incoming-packets" time_duration(0,0,0,0))
    void setMaxSampleAge(const std::string& name,
                         boost::posix_time::time_duration duration);

    /// @brief determines how many samples of a given statistic should be kept.
    ///
    /// Specifies that statistic name should be stored not as single value, but
    /// rather as a set of values. In this form, at most max_samples will be kept.
    /// When adding max_samples+1 sample, the oldest sample will be discarded.
    ///
    /// @todo: Not implemented.
    ///
    /// Example:
    /// To set a statistic to keep the last 100 observations, call:
    /// setMaxSampleCount("incoming-packets", 100);
    void setMaxSampleCount(const std::string& name, uint32_t max_samples);

    /// @brief Resets specified statistic.
    ///
    /// This is a convenience function and is equivalent to setValue(name,
    /// neutral_value), where neutral_value is 0, 0.0 or "".
    /// @param name name of the statistic to be reset.
    /// @return true if successful, false if there's no such statistic
    bool reset(const std::string& name);

    /// @brief Removes specified statistic.
    /// @param name name of the statistic to be removed.
    /// @return true if successful, false if there's no such statistic
    bool del(const std::string& name);

    /// @brief Resets all collected statistics back to zero.
    void resetAll();

    /// @brief Removes all collected statistics.
    void removeAll();

    /// @brief Returns number of available statistics.
    /// @return number of recorded statistics.
    size_t count() const;

    /// @brief Returns a single statistic as a JSON structure
    /// @return JSON structures representing a single statistic
    isc::data::ConstElementPtr get(const std::string& name) const;

    /// @brief Returns all statistics as a JSON structure
    /// @return JSON structures representing all statistics
    isc::data::ConstElementPtr getAll() const;

    /// @brief Returns an observation
    ///
    /// Used in testing only. Production code should use @ref get() method.
    /// @param name name of the statistic
    /// @return Pointer to the Observation object
    ObservationPtr getObservation(const std::string& name) const;

 private:

    template<typename DataType>
    void setValueInternal(const std::string& name, DataType value) {
        ObservationPtr stat = getObservation(name);
        if (stat) {
            stat->setValue(value);
        } else {
            stat.reset(new Observation(name, value));
            addObservation(stat);
        }
    }

    template<typename DataType>
    void addValueInternal(const std::string& name, DataType value) {
        ObservationPtr existing = getObservation(name);
        if (!existing) {
            // We tried to add to a non-existing statistic. We can recover from
            // that. Simply add the new incremental value as a new statistic and
            // we're done.
            setValue(name, value);
            return;
        } else {
            // Let's hope it is of correct type. If not, the underlying
            // addValue() method will throw.
            existing->addValue(value);
        }
    }

    /// @brief Private constructor
    /// StatsMgr is a singleton. It should be accessed using @ref instance
    /// method.
    StatsMgr();

    /// @brief Adds a new observation
    ///
    /// That's an utility method used by public @ref setValue() and
    /// @ref addValue() methods.
    /// @param obs observation
    void addObservation(const ObservationPtr& o);

    /// @brief Tries to delete an observation
    ///
    /// @param name of the statistic to be deleted
    /// @return true if deleted, false if not found
    bool deleteObservation(const std::string& name);

    // This is a global context. All stats will initially be stored here.
    StatContextPtr global_;
};

};
};

#endif // STATS_MGR
