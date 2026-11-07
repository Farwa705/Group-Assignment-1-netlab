#include"net.hpp"
#include<iostream>

int main()
{
  net::init();
sock_t listener = net::tcp_listen(7000);
std::cout<<"Echo server listening on port 7000...\n";

while(true)
{
sock_t client = net::tcp_accept(listening);
std::cout<<"Client connected!\n";

char buff[1024];
int n;
while((n = net::recv_some(client, buf, sizeof(buf)))>0)
{
net::send_all(client, std::string(buf, n));
}
net::close_socket(client);
std::cout<<"Client disconnected.\n";
}
net::close_socket(listener);
net::cleanup();
return 0;
}
