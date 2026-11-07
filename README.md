# Computer Networks — Assignment 1

A C++ network programming repository containing socket applications built over a cross platform abstraction layer (`net.hpp`). Implements servers and clients for Daytime, Echo, Web, Framing, Threaded, and a multi-user Capstone Chat System.

## 1. Project Structure

* `net.hpp` - Core cross-platform socket header (handles Windows Winsock2 and Linux POSIX abstraction)
* `daytime_server.cpp` & `daytime_client.cpp` - Daytime server & client (Port 9000)
* `echo_server.cpp` & `echo_client.cpp` - Echo server & client (Port 7000)
* `web_server.cpp` - HTTP static web server (Port 8080)
* `frame_server.cpp` & `frame_client.cpp` - TCP framing & stream message handling (Port 7100)
* `thread_server.cpp` - Multi-threaded server (Port 7200)
* `chat_server.cpp` & `chat_client.cpp` - Multi-user select-based chat server & dual-threaded client (Port 6000)
* `screenshots/` - Verification screenshots for all lab tasks
* `www/` - Static HTML assets for the web server

## 2. Compilation Instructions

Compile all files using MinGW `g++` on Windows (or standard `g++` on Linux). 

> **Note:** On Windows, link the Winsock library using `-lws2_32`. Multithreaded programs require `-std=c++17`.

```cmd
g++ daytime_server.cpp -o daytime_server.exe -lws2_32
g++ daytime_client.cpp -o daytime_client.exe -lws2_32

g++ echo_server.cpp -o echo_server.exe -lws2_32
g++ echo_client.cpp -o echo_client.exe -lws2_32

g++ web_server.cpp -o web_server.exe -lws2_32

g++ frame_server.cpp -o frame_server.exe -lws2_32 -std=c++17
g++ frame_client.cpp -o frame_client.exe -lws2_32 -std=c++17

g++ thread_server.cpp -o thread_server.exe -lws2_32 -std=c++17

g++ chat_server.cpp -o chat_server.exe -lws2_32 -std=c++17
g++ chat_client.cpp -o chat_client.exe -lws2_32 -std=c++17
