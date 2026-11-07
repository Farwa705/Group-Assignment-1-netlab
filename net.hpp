#pragma once
#include <string>
#include <cstring>
#include <stdexcept>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  typedef SOCKET sock_t;
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  typedef int sock_t;
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  #define closesocket close
#endif

namespace net {

inline void init() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
}

inline void cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

inline sock_t tcp_listen(int port) {
    sock_t s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    bind(s, (sockaddr*)&addr, sizeof(addr));
    listen(s, 16);
    return s;
}

inline sock_t tcp_accept(sock_t listener) {
    return accept(listener, nullptr, nullptr);
}

inline sock_t tcp_connect(const std::string& host, int port) {
    sock_t s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
    connect(s, (sockaddr*)&addr, sizeof(addr));
    return s;
}

inline bool send_all(sock_t s, const std::string& data) {
    const char* buf = data.c_str();
    size_t left = data.size();
    while (left > 0) {
        int n = send(s, buf, (int)left, 0);
        if (n <= 0) return false;
        buf += n;
        left -= n;
    }
    return true;
}

inline int recv_some(sock_t s, char* buf, int len) {
    return recv(s, buf, len, 0);
}

inline void close_socket(sock_t s) {
    closesocket(s);
}

} // namespace net

