#ifndef TEMPLATE_SERVER_H
#define TEMPLATE_SERVER_H
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include "Client.h"
namespace ip=boost::asio::ip;
class Server {
private:
    boost::asio::io_service service;
    void accept_thread();
    std::vector<Client> clients;
    void handle_clients_thread();
};


#endif //TEMPLATE_SERVER_H
