#include "net.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string& path, bool& found) {
    std::ifstream f(path, std::ios::binary);
    if (!f) { found = false; return ""; }
    found = true;
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main() {
    net::init();
    sock_t listener = net::tcp_listen(8080);
    std::cout << "Web server listening on http://127.0.0.1:8080\n";

    while (true) {
        sock_t client = net::tcp_accept(listener);
        char buf[8192];
        int n = net::recv_some(client, buf, sizeof(buf) - 1);
        if (n <= 0) { net::close_socket(client); continue; }
        buf[n] = '\0';

        std::string request(buf);
        std::string path = "/";
        size_t start = request.find(' ');
        size_t end = request.find(' ', start + 1);
        if (start != std::string::npos && end != std::string::npos)
            path = request.substr(start + 1, end - start - 1);
        if (path == "/") path = "/index.html";

        bool found;
        std::string body = readFile("www" + path, found);
        std::string status = found ? "200 OK" : "404 Not Found";
        if (!found) body = "<h1>404 Not Found</h1>";

        std::ostringstream response;
        response << "HTTP/1.1 " << status << "\r\n"
                 << "Content-Length: " << body.size() << "\r\n"
                 << "Connection: close\r\n\r\n" << body;

        net::send_all(client, response.str());
        net::close_socket(client);
    }

    net::close_socket(listener);
    net::cleanup();
    return 0;
}

