# ChatApp - A Multithreaded Chat Server

## Overview
ChatApp is a simple multithreaded chat server written in C++ that allows multiple clients to connect and chat simultaneously. It utilizes:
- **Sockets** for network communication
- **Threads** for handling multiple clients
- **SQLite3** for storing chat messages
- **Google Test (gtest)** for unit testing
- **CMake & CPack** for building and packaging

## Features
✅ Supports multiple clients (at least 5)  
✅ Stores messages in a SQLite3 database  
✅ Responds with fun messages  
✅ Uses Google Test for unit testing  
✅ Packaged using CMake & CPack  

## Prerequisites
Ensure you have the following installed on your system:
- **C++ Compiler (g++ or clang++)**
- **CMake (3.10 or later)**
- **SQLite3**
- **Google Test**
- **Make**
- **Git**

### Install Dependencies (Linux)
```bash
sudo apt update && sudo apt install -y build-essential cmake libsqlite3-dev libgtest-dev
```

## 1️⃣ Build the Project

Run the following commands to build the project:
```bash
mkdir -p build && cd build
cmake ..
make
```

## 2️⃣ Running the Server
Once you have built the project, you can run the server:
```bash
./ChatApp
```
By default, it listens on port **12345**.

## 3️⃣ Testing the Server with Netcat
Open a new terminal and connect to the server:
```bash
nc 127.0.0.1 12345
```
Now you can type messages and interact with the chat server. Type `quit` to close the connection.

## 4️⃣ Verifying Database Storage
Chat messages are stored in an SQLite3 database. To verify stored messages:
```bash
sqlite3 chatapp.db "SELECT * FROM messages;"
```
This will display all stored chat messages.

## 5️⃣ Running Unit Tests (Google Test)
To run unit tests:
```bash
cd build
ctest --output-on-failure
```
Or manually run the test binary:
```bash
./test_chatapp
```

## 6️⃣ Using Makefile for Building
If your project includes a Makefile, you can build it using:
```bash
make
```
To clean the build:
```bash
make clean
```

## 7️⃣ Packaging the Application (CMake & CPack)
To generate a package:
```bash
cd build
cpack
```
This will create a distributable package that you can share or deploy.

## 8️⃣ Contributing
Feel free to fork this repository, create feature branches, and submit pull requests!

## 9️⃣ Troubleshooting
- **Port Already in Use?**
  ```bash
  sudo netstat -tulnp | grep 12345
  sudo kill -9 <PID>
  ```
- **Database Not Updating?**
  ```bash
  sqlite3 chatapp.db "PRAGMA integrity_check;"
  ```

## 📜 License
This project is licensed under the MIT License.

---
Happy Coding! 🚀

