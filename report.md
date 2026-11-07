
# Computer Networks - Assignment 1 Report

**Course:** Computer Networks

**Title:** C++ Socket Programming & Multi-User Chat System

**Members:**
Hania Zafar - 250901028
Farwa Zainab - 250901069
Nayab Fatima - 250901024
Aimen Aatif - 250901049
Warisha Bukhari - 250901005

---

## 1. Introduction

For this assignment, we were tasked with building a complete set of network applications from scratch using C++ sockets. The core goal was to understand how network protocols work at the transport layer by implementing everything from a simple one-shot server to a multi-threaded server and a multi-client chat system.

## 2. System Architecture & The `net.hpp` Helper

Writing network code that works on both Windows and Linux can get messy very quickly because of how different operating systems handle socket initialization and cleanup.

* To fix this, we put all OS-specific `#ifdef` blocks inside a single header file called `net.hpp`.
* On **Windows (`_WIN32`)**, the library automatically calls `WSAStartup()` during `net::init()` and uses the `SOCKET` type along with `closesocket()`.
* On **Linux / POSIX**, it uses standard integer file descriptors and standard `close()` calls.
* This allowed all our servers and clients to call clean wrapper functions like `net::tcp_listen()`, `net::tcp_connect()`, `net::send_all()`, and `net::recv_some()` without worrying about what operating system they were running on.

## 3. Program Implementations & Walkthrough

### 3.1 Daytime Server & Client (Port 9000)

* **What it does:** This is the most basic TCP server cycle. It listens on port 9000, waits for a client to connect, grabs the current system time using `ctime()`, sends that string back to the client, and immediately closes the connection.
* **How we tested it:** We ran the server in one terminal and ran the client in another to verify it successfully received and printed the current date and time.

### 3.2 Echo Server & Client (Port 7000)

* **What it does:** An iterative echo server. When a client connects, any text the client sends is read into a buffer and sent right back ("echoed") until the client disconnects.
* **How we tested it:** We typed various lines into the client terminal and verified that the server echoed every single line back correctly.

### 3.3 Static Web Server (Port 8080)

* **What it does:** A lightweight HTTP/1.0 server. It reads incoming browser requests, parses out the requested file path (defaulting to `index.html`), reads it from a local `www/` folder, and returns it with proper HTTP headers (`200 OK` or `404 Not Found` if the file doesn't exist).
* **How we tested it:** We tested it by pointing a web browser to `[http://127.0.0.1:8080](http://127.0.0.1:8080)` and verifying that our HTML page loaded correctly.

### 3.4 Framing Server & Client (Port 7100)

* **What it does:** This program demonstrates a classic TCP problem: TCP is a continuous byte stream, meaning data can arrive split into pieces or merged together. To handle this, the server uses a persistent string buffer (`inbuf`) and looks for a newline delimiter (`\n`) to safely parse out complete messages.
* **How we tested it:** The client deliberately sent messages in split pieces (e.g., `"Hello"` followed by `" World\n"`), and we verified that the server successfully waited and printed the complete joined message.

### 3.5 Threaded Server (Port 7200)

* **What it does:** To prevent a slow client from blocking everyone else, this server uses C++11 threads (`std::thread`). Every time a new client connects via `accept()`, it spawns a dedicated worker thread and detaches it so it can handle that client independently.
* **How we tested it:** We opened multiple `ncat` terminals at the same time and connected them to the server to make sure multiple clients could interact with it concurrently.

### 3.6 Multi-User Chat System (Port 6000) — Capstone

* **What it does:** This is the main project requirement.
* **The Server (`chat_server.cpp`)** uses `select()` in a single thread to monitor multiple client sockets at the same time without dealing with thread synchronization locks. It assigns default names (`user1`, `user2`), tracks input buffers for each user, and broadcasts messages. It also supports chat commands like `/nick <name>`, `/who`, and `/quit`.
* **The Client (`chat_client.cpp`)** uses two threads: one main thread for typing and sending messages, and a background thread (`receiver`) that continuously listens for incoming messages from the server and prints them live.


* **How we tested it:** We ran the server and opened multiple client terminal windows to test chatting, changing nicknames, checking online users with `/who`, and quitting gracefully.

## 4. Compilation Commands

To compile all programs using MinGW `g++` on Windows, make sure to link the Winsock library (`-lws2_32`) and enable C++17 (`-std=c++17`) for threaded programs:

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

## 5. Verification & Testing Evidence

Screenshots showing the successful compilation and execution of all servers, clients, and chat features are saved in the repository's `screenshots/` directory.
