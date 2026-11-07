#include "net.hpp"
#include<iostream>
#include<string>

int main()
{
  net::init();
sock_t s = net::tcp_connect("127.0.0.1",7000);

std::string line;
std::cout<<"Type a message(empty line to quit):\n";
while(std::getline(std::cin, line)) 
{
if(line.empty()) break;
net::send_all(s,line);

char buf[1024];
int n=net::recv_some(s,buf, sizeof(buf)-1);
if(n<=0)break;
buf[n]='\0';
std::cout<<"Echo:"<<buf<<"\n";
}
net::close_socket(s);
net::cleanup();
return 0;
}
