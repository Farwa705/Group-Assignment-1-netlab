#include "net.hpp"
#include <iostream>
#include <thread>
#include <chrono>
 
int main() {
    net::init();
    sock_t s = net::tcp_connect("127.0.0.1", 7100);
 
    net::send_all(s, "Hello");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    net::send_all(s, " World\n");
 
    net::send_all(s, "Second message\nThird message\n");
 
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    net::close_socket(s);
    net::cleanup();
    return 0;
}
