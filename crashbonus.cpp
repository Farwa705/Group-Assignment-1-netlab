#include "net.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    net::init();
    std::cout << "Connecting to the web server...\n";
    sock_t s = net::tcp_connect("127.0.0.1", 8080);
    std::cout << "Connected - staying silent for 30 seconds. Server should now be stuck.\n";

    std::this_thread::sleep_for(std::chrono::seconds(30));

    net::close_socket(s);
    net::cleanup();
    std::cout << "Done, socket closed.\n";
    return 0;
}
