//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once
//
// base sink templated over a mutex (either dummy or real)
// concrete implementation should only override the _sink_it method.
// all locking is taken care of here so no locking needed by the implementers..
//

#include "sink.h"
#include "../formatter.h"
#include "../common.h"
#include "../details/log_msg.h"

#include <mutex>

namespace spdlog
{
namespace sinks
{
template<class Mutex>
class base_sink:public sink
{
public:
    base_sink():_mutex() {}
    virtual ~base_sink() = default;

    base_sink(const base_sink&) = delete;
    base_sink& operator=(const base_sink&) = delete;

    void log(const details::log_msg& msg) SPDLOG_FINAL override
    {
        std::lock_guard<Mutex> lock(_mutex);
        _sink_it(msg);
    }
    void flush() SPDLOG_FINAL override
    {
        std::lock_guard<Mutex> lock(_mutex);
        _flush();
    }

    int copy_to(const path_t& /*destination*/, bool /*silent*/) override
    {
        //do nothing
        return 0;
    }

    filename_t get_destination() const override
    {
        return filename_t();
    }

protected:
    virtual void _sink_it(const details::log_msg& msg) = 0;
    virtual void _flush() = 0;
    Mutex _mutex;
};
}
}
