#include "net.hpp"
#include <iostream>
#include <string>
 
int main() {
    net::init();
    sock_t listener = net::tcp_listen(7100);
    std::cout << "Frame server listening on port 7100...\n";
 
    sock_t client = net::tcp_accept(listener);
    std::cout << "Client connected!\n";
 
    std::string inbuf;
    char buf[1024];
    int n;
    while ((n = net::recv_some(client, buf, sizeof(buf))) > 0) {
        inbuf.append(buf, n);
        size_t pos;
        while ((pos = inbuf.find('\n')) != std::string::npos) {
            std::string line = inbuf.substr(0, pos);
            if (!line.empty() && line.back() == '\r') line.pop_back();
            inbuf.erase(0, pos + 1);
            std::cout << "Received full message: " << line << "\n";
        }
    }

    net::close_socket(client);
    net::close_socket(listener);
    net::cleanup();
    return 0;
}

