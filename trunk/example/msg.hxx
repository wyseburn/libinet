#ifndef __LIBINET_TEST_TIME_MSG_H__
#define __LIBINET_TEST_TIME_MSG_H__

#include <string>
#include "message.hxx"

struct get_time_req
{
    std::string username_; 
};

static const int get_time_req_id = 1;
INET_MESSAGE_1(get_time_req_id, get_time_req, std::string, username_);

struct get_time_res
{
    unsigned int server_id_;
    std::string time_; 
};

static const int get_time_res_id = 2;
INET_MESSAGE_2(get_time_res_id, get_time_res, unsigned int, server_id_, std::string, time_);

#endif // #ifndef __LIBINET_TEST_TIME_MSG_H__

