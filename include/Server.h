// Copyright 2020 <>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <boost/thread.hpp>
#include <deque>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include "log.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using boost::asio::ip::tcp;
using boost::system::error_code;
const int failTime = 5;
const unsigned int max_msg = 1024;
const int PORT = 8001;
const std::chrono::milliseconds second = std::chrono::milliseconds(1000);

struct Client {
    tcp::socket socket;
    std::string login;
    std::chrono::system_clock::time_point lastLogin;
    boost::asio::streambuf buffer;
    explicit Client(boost::asio::io_context& io_context):socket(io_context),
                                                        login(),
                                                        lastLogin(std::chrono::system_clock::now()),
                                                        buffer()
                                                        {
    }
};
class Server{
public:
    Server();
private:
    boost::asio::io_context ioService;
    std::vector<std::shared_ptr<Client>> clients;
    std::recursive_mutex mutex;
    int already_read_ = 0;
    char buff[max_msg];
    bool clients_changed;
    void runner();
    void on_ping(std::shared_ptr<Client>& client);
    void on_login(const std::string & msg, std::shared_ptr<Client>& client);
    void on_clients(std::shared_ptr<Client>& client);
    void answer_to_client( std::shared_ptr<Client>& client);
    void connectionHandler();
    void clientHandler();
    };
#endif // INCLUDE_HEADER_HPP_
