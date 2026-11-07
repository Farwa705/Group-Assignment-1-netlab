#include "net.hpp"
#include <iostream>
 
int main() {
    net::init();
    sock_t s = net::tcp_connect("127.0.0.1", 9000);
 
    char buf[1024];
    int n = net::recv_some(s, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = '\0';
        std::cout << "Server says: " << buf;
    }
 
    net::close_socket(s);
    net::cleanup();
    return 0;
}

