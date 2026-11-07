#include "net.hpp"
#include <iostream>
#include <thread>

void handleClient(sock_t client) {
    char buf[1024];
    int n;
    while ((n = net::recv_some(client, buf, sizeof(buf))) > 0) {
        net::send_all(client, std::string(buf, n));
    }
    net::close_socket(client);
    std::cout << "A client disconnected.\n";
}

int main() {
    net::init();
    sock_t listener = net::tcp_listen(7200);
    std::cout << "Threaded server listening on port 7200...\n";

    while (true) {
        sock_t client = net::tcp_accept(listener);
        std::cout << "New client connected, starting a worker thread.\n";
        std::thread t(handleClient, client);
        t.detach();
    }

    net::close_socket(listener);
    net::cleanup();
    return 0;
}

