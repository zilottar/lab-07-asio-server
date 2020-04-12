//Copyright[year]<Copyright Owner>
#include "Client.h"
Client::Client() {
    std::cout << "Введите ваше имя пользователя";
    std::cin >> this->username;
    this->socket.reset(new ip::tcp::socket{this->service});
}
ip::tcp::socket& Client::sock() {
    return *this->socket.get();
}
void Client::answer_to_client() {
    Client::ping_process();
    boost::posix_time::ptime now=boost::posix_time::microsec_clock::local_time();
    if((now-last_time).total_milliseconds() > 5000){
        boost::system::error_code err;
        sock().close(err);
    }
}
void Client::ping_process() {
    if(sock().available()){
        already_read+= sock().read_some(
                boost::asio::buffer(buff + already_read, 
                    max_size - already_read));
    }
}
