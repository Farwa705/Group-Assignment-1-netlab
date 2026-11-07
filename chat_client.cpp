#include "net.hpp"
#include <iostream>
#include <thread>
#include <string>

void receiver(sock_t s)
{
    char buf[1024];
    while (true)
    {
        int n = net::recv_some(s, buf, sizeof(buf) - 1);
        if (n <= 0)
        {
            std::cout << "\nDisconnected from server.\n";
            break;
        }
        buf[n] = '\0';
        std::cout << buf;
    }
}

int main()
{
    net::init();
    sock_t s = net::tcp_connect("127.0.0.1", 6000);

    std::thread rt(receiver, s);

    std::string line;
    while (std::getline(std::cin, line))
    {
        line += "\n";
        if (!net::send_all(s, line))
            break;
        if (line == "/quit\n")
            break;
    }

    net::close_socket(s);
    rt.join();
    net::cleanup();
    return 0;
}
