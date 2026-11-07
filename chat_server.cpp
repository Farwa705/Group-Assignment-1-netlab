#include "net.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

struct Client
{
    sock_t sock;
    std::string name;
    std::string inbuf;
};

std::vector<Client> clients;
int nextId = 1;

bool validName(const std::string &n)
{
    if (n.empty() || n.size() > 16)
        return false;
    for (char c : n)
        if (!isalnum((unsigned char)c) && c != '_')
            return false;
    for (auto &c : clients)
        if (c.name == n)
            return false;
    return true;
}

void broadcast(const std::string &msg, sock_t except = INVALID_SOCKET)
{
    for (auto &c : clients)
        if (c.sock != except)
            net::send_all(c.sock, msg);
}

void handleLine(Client &c, std::string line)
{
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    if (line.empty())
        return;

    if (line.rfind("/nick ", 0) == 0)
    {
        std::string newName = line.substr(6);
        if (validName(newName))
        {
            std::string old = c.name;
            c.name = newName;
            net::send_all(c.sock, "Nickname changed to " + newName + "\n");
            broadcast(old + " is now " + newName + "\n");
        }
        else
        {
            net::send_all(c.sock, "Invalid or taken name.\n");
        }
    }
    else if (line == "/who")
    {
        std::string list = "Online: ";
        for (auto &o : clients)
            list += o.name + " ";
        list += "\n";
        net::send_all(c.sock, list);
    }
    else if (line == "/quit")
    {
        net::close_socket(c.sock);
        c.sock = INVALID_SOCKET;
    }
    else
    {
        broadcast(c.name + ": " + line + "\n", c.sock);
    }
}

int main()
{
    net::init();
    sock_t listener = net::tcp_listen(6000);
    std::cout << "Chat server listening on port 6000...\n";

    while (true)
    {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);
        sock_t maxfd = listener;

        for (auto &c : clients)
        {
            FD_SET(c.sock, &readset);
            if (c.sock > maxfd)
                maxfd = c.sock;
        }

        select((int)maxfd + 1, &readset, nullptr, nullptr, nullptr);

        if (FD_ISSET(listener, &readset))
        {
            sock_t s = net::tcp_accept(listener);
            Client c;
            c.sock = s;
            c.name = "user" + std::to_string(nextId++);
            clients.push_back(c);
            broadcast(c.name + " joined\n");
            std::cout << c.name << " connected\n";
        }

        for (auto &c : clients)
        {
            if (c.sock != INVALID_SOCKET && FD_ISSET(c.sock, &readset))
            {
                char buf[1024];
                int n = net::recv_some(c.sock, buf, sizeof(buf) - 1);
                if (n <= 0)
                {
                    net::close_socket(c.sock);
                    c.sock = INVALID_SOCKET;
                }
                else
                {
                    buf[n] = '\0';
                    c.inbuf += buf;
                    size_t pos;
                    while ((pos = c.inbuf.find('\n')) != std::string::npos)
                    {
                        std::string line = c.inbuf.substr(0, pos);
                        c.inbuf.erase(0, pos + 1);
                        handleLine(c, line);
                    }
                }
            }
        }

        auto it = std::remove_if(clients.begin(), clients.end(),
                                 [](Client &c)
                                 { return c.sock == INVALID_SOCKET; });
        for (auto gone = it; gone != clients.end(); ++gone)
            broadcast(gone->name + " left\n");
        clients.erase(it, clients.end());
    }

    net::cleanup();
    return 0;
}
