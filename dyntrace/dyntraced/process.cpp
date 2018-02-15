#include "process.hpp"

#include <boost/log/trivial.hpp>

using namespace dyntrace::d;
using namespace dyntrace::proto;
using namespace dyntrace::proto::process;

void process_connection::on_hello(uint64_t seq, const hello& h)
{
   BOOST_LOG_TRIVIAL(info) << "hello: " << h.pid();
}

void process_connection::on_bye(uint64_t seq, const bye& b)
{
    BOOST_LOG_TRIVIAL(info) << "bye";
}

response process_connection::on_request(uint64_t seq, const request& req)
{
    BOOST_LOG_TRIVIAL(info) << "request";
    response resp;
    resp.mutable_ok();
    return resp;
}