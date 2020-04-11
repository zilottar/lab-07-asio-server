
#include "Server.h"


void Server::accept_thread()
{
    ip::tcp::acceptor acceptor(this->service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
    while (true)
    {
        std::shared_ptr<Client> client(new Client);
        acceptor.accept(client.get()->sock());
        clients.push_back(*client);
    }
}
void Server::handle_clients_thread()
{
    while ( true)
    {
        boost::this_thread::sleep(std::chrono::milliseconds{1});
        for (auto& client : clients) {
            client.answer_to_client();
        // erase clients that timed out
        clients.erase(std::remove_if(clients.begin(), clients.end(),
                                     boost::bind(&talk_to_client::timed_out,_1)), clients.end());
    }
}
