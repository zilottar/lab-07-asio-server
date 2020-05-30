// Copyright 2020 <>

#include <server.h>

void Server::connectionHandler(){
  tcp::acceptor acceptor(ioService,
  boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), PORT});
  while (true) {
        auto client = std::make_shared<Client>(ioService);
        acceptor.accept(client -> socket);
        boost::lock_guard<std::recursive_mutex> lock(mutex);
        clients.push_back(client);
    }
}
void Server::on_login(const std::string & msg, std::shared_ptr<Client>& client)
{
  std::lock_guard<std::recursive_mutex> lock(mutex);
    client->login = std::string(msg, 6);
    logInfoClientsMessage("Client logged in: " + client->login);
    client->socket.write_some(boost::asio::buffer("login ok\n"));
    clients_changed = true;
}

void Server::on_ping(std::shared_ptr<Client>& client)
{
  std::lock_guard<std::recursive_mutex> lock(mutex);
    std::string s = (clients_changed ?
    "ping client_list_changed\n" : "ping ok\n");
    client->socket.write_some(boost::asio::buffer(s));
    clients_changed = false;
}

void Server::on_clients(std::shared_ptr<Client>& client)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);
    std::string msg;
    {
        for (auto &client : clients) {
            msg += (client)->login + " ";
        }
    }
    logInfoClientsMessage(client->login + " asked clients list");
    client->socket.write_some(boost::asio::buffer("clients " + msg + "\n"));
}

void Server::answer_to_client(std::shared_ptr<Client>& client) {
    try {
        if (client->socket.available()) {
            already_read_ += client->socket.read_some(
                    boost::asio::buffer(buff + already_read_,
                                  max_msg - already_read_));
        }
      bool found_enter = std::find(buff, buff + already_read_,
                                   '\n') < buff + already_read_;
      if (!found_enter)
        return;
      client->lastLogin = std::chrono::system_clock::now();
      size_t pos = std::find(buff, buff + already_read_, '\n') - buff;
      std::string msg(buff, pos);
      std::copy(buff + already_read_, buff + max_msg, buff);
      already_read_ -= pos + 1;
      if ( msg.find("login") == 0) on_login(msg, client);
      else if ( msg.find("ping") == 0) on_ping(client);
      else if ( msg.find("list") == 0) on_clients(client);
      else
        client->socket.write_some(boost::asio::buffer("Invalid message\n"));
    } catch (error_code& err) {
      client->socket.close(err);
    }
}

void Server::clientHandler(){
    while (true) {
    std::this_thread::sleep_for(second);
      std::lock_guard<std::recursive_mutex> lock(mutex);
      for (auto iter = clients.begin(); iter != clients.end(); ++iter) {
        (*iter)->socket.non_blocking(true);
        answer_to_client(*iter);
      }
      std::vector<std::shared_ptr<Client>> *clients_new =
          new std::vector<std::shared_ptr<Client>>;
      for (auto iter = clients.begin(); iter != clients.end(); ++iter) {
        if (std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()) -
        std::chrono::system_clock::to_time_t((*iter)->lastLogin) <
        failTime){
          clients_new->push_back(*iter);
        } else {
          clients_changed = true;
          logInfoClientsMessage((*iter)->login+" terminated");
        }
      }
      clients = (*clients_new);
      delete clients_new;
    }
}

Server::Server() {
  runner();
}
void Server::runner(){
  boost::thread t1(&Server::connectionHandler, this);
  boost::thread t2(&Server::clientHandler, this);
  t1.join();
  t2.join();
}
