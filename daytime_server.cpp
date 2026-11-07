#include "net.hpp"
#include <iostream>
#include <ctime>
 
int main() {
    net::init();
    sock_t listener = net::tcp_listen(9000);
    std::cout << "Daytime server listening on port 9000...\n";
 
    while (true) {
        sock_t client = net::tcp_accept(listener);
        std::cout << "Client connected!\n";
 
        time_t now = time(nullptr);
        std::string msg = ctime(&now);
        net::send_all(client, msg);
 
        net::close_socket(client);
    }
 
    net::close_socket(listener);
    net::cleanup();
    return 0;
}

