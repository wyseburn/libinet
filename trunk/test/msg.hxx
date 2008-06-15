

#ifndef __INET_TEST_MSG_H__
#define __INET_TEST_MSG_H__

#include <string>
#include "message.hxx"

struct echo_req
{
    unsigned int id_;
    std::string msg_;
};

INET_MESSAGE_2(1, echo_req, unsigned int, id_, std::string, msg_); 

#endif // #ifndef __INET_TEST_MSG_H__
