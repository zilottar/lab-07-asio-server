#ifndef TEMPLATE_CLIENT_H
#define TEMPLATE_CLIENT_H
#include <iostream>
#include <thread>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
namespace ip=boost::asio::ip;
const int max_size = 1024;
class Client {
public:
    Client();
    ip::tcp::socket& sock();
    void answer_to_client();
    boost::posix_time::ptime last_time;
private:
    std::shared_ptr<ip::tcp::socket> socket;
    boost::asio::io_service service;
    std::string username;
    void ping_process();
    char buff[max_size];
    int already_read;
};


#endif //TEMPLATE_CLIENT_H
