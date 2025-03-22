# ChatApp - A Multithreaded Chat Server

## Overview
ChatApp is a simple multithreaded chat server written in C++ that allows multiple clients to connect and chat. It uses:
- **Sockets** for network communication
- **Threads** for handling multiple clients
- **SQLite3** for storing chat messages
- **Google Test** for unit testing
- **CMake & CPack** for building and packaging

## Features
✅ Supports multiple clients (at least 5)  
✅ Stores messages in a SQLite3 database  
✅ Responds with fun messages  
✅ Uses Google Test for unit testing  
✅ Packaged using CMake & CPack  

## Getting Started

### 1️⃣ **Build the Project**
```bash
mkdir -p build && cd build
cmake ..
make

## Running the Server

Once you have built the project, you can run the server:

```bash
./ChatApp

### ** 2️⃣ To test the server using netcat, open another terminal and run **
```bash

nc 127.0.0.1 12345

### ** 3️⃣ Send messages and interact with the server! **

Type any message and get a fun response.

Type quit to close the connection.


