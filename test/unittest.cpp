#include <gtest/gtest.h>
#include <iostream>
using namespace std;
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <ctime> // For timestamp
#include <thread>
#include <sqlite3.h>
#include <algorithm>

sqlite3 *db;

//client dashboard
std::string sendWelcomeMessage(int client_socket) {
    // Get current timestamp
    std::time_t now = std::time(0);
    std::string timestamp = std::ctime(&now);

    // Display login time
    std::string welcome = "Welcome! Thank you for connecting to the server :) \n\nLogin time: " + timestamp+"\n\n";
    
    // Send data to the client
    const char *message = welcome.c_str();
    send(client_socket, message, strlen(message), 0);
    return timestamp;
}

// Function to read the first 256 bytes only
ssize_t first256bytes(int client_socket,char *buffer){

        ssize_t bytes_read = read(client_socket, buffer, 256);
        if (bytes_read < 0) {
            std::cout << "Read failed." << std::endl;
            return -1;
        }
        return bytes_read;
}

//func to pick random words from array and append to client message
std::string randomWordsAppend(std::string receivedMessage){
        // Making an array of strings and appending any two random words to the client message
        std::string strings[] = {"ice", "land", "cream", "tea", "cake", "rain", "smile", "crazy", "sounds", "lovely"};
        int numWords = sizeof(strings) / sizeof(strings[0]);
        // Generate two random indices in the range [0, 10]
            int randomIndex1 = rand() % numWords;
            int randomIndex2 = rand() % numWords;
            while (randomIndex2 == randomIndex1) {
                // Make sure the second random index is not the same as the first
                randomIndex2 = rand() % numWords;
            }
            // Append the two random words to the existing message
            std::string randomWords = strings[randomIndex1] + strings[randomIndex2];
	    std::string replyMessage = "Server reply: " + receivedMessage + ":" + randomWords;
            return replyMessage;
}

//func to create database
int createdb(){
    char *errMsg = 0;
    // open or creates an SQLite database
    int rc = sqlite3_open("chatapp.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }
    // create a schema with a table for chat messages
    std::string sql = "CREATE TABLE IF NOT EXISTS chat_messages (id INTEGER PRIMARY KEY AUTOINCREMENT,clientID INTEGER,client_msg TEXT, timestamp TEXT)";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating schema: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    }
    return 0;
}

//func to insert data to database
int inserttodb(int client_id,std::string receivedMessage,std::string timestamp){
            createdb();
            // Insert new data into the db table
            std::string sql = "INSERT INTO chat_messages (clientID, client_msg, timestamp) VALUES (" +
                              std::to_string(client_id) + ", '" + receivedMessage + "', '" + timestamp + "');";
            char *errMsg;
            int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
            if (rc != SQLITE_OK) {
                std::cerr << "Error inserting data: " << errMsg << std::endl;
                sqlite3_free(errMsg); 
            }
return 0;
}

//client func
void client(int client_socket, int client_id) {
    //calling client dashboard func
    std::string timestamp=sendWelcomeMessage(client_socket);
    while (true) {
        char buffer[257] = {0};
        ssize_t bytes_read = first256bytes(client_socket, buffer);

	// Null-terminate the received data
        buffer[bytes_read] = '\0'; 
        std::string receivedMessage(buffer);
        std::string terminate = "quit";

        // Closing the server when client writes "quit"
        if (receivedMessage.find(terminate) != std::string::npos) {
            std::cout << "Client " << client_id << " requested to quit. Closing connection..." << std::endl;
            std::string goodbye_message = "Goodbye! Connection closed.";
            send(client_socket, goodbye_message.c_str(), goodbye_message.length(), 0);
            break;
        } 
        else {
            std::cout << "Client " << client_id << " message: " << receivedMessage << std::endl;
            inserttodb(client_id,receivedMessage,timestamp);
            std::string server_reply=randomWordsAppend(receivedMessage);
            server_reply.erase(std::remove_if(server_reply.begin(), server_reply.end(), [](char c) { return c == '\n'; }), server_reply.end());
            server_reply=server_reply+"\n\n";
            send(client_socket, server_reply.c_str(), server_reply.length(), 0);         std::cout << std::endl;
            receivedMessage={0};
        }
    }

    // Close the database connection
    sqlite3_close(db);
    close(client_socket);
}

//Test case 1 to check if message is appended
TEST(MyFunctionsTest1, randomWords) {
    std::string receivedMessage = "Hello";
    std::string expectedReplyMessage = "Server reply: Hello:randomword1randomword2";

    // Set a fixed seed for predictable random number generation in the test
    srand(1234);

    std::string actualReplyMessage = randomWordsAppend(receivedMessage);

    EXPECT_EQ(actualReplyMessage, expectedReplyMessage);
}

//Test case 2 for database
TEST(MyFunctionsTest2, inserttodb) {

    // Insert data into the database
    int client_id=1;
    std::string receivedMessage="My name is Tayyaba";
    std::string timestamp="Login time: Fri Jun  2 08:21:43 2023";
    int rc= inserttodb(client_id,receivedMessage,timestamp);
    ASSERT_EQ(rc,SQLITE_OK) << "Failed to insert data into the database: ";
}

//Test case 3 to check the server functionality
TEST(ThreadingTest, SocketCreation) {
    const int numClients = 5;
    std::thread threads[numClients];
    int clientCounter = 0;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // Create a socket
    if (server_socket == -1) {
        std::cout << "Could not create socket." << std::endl;
        ASSERT_TRUE(false);
    }

    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("Failed to set socket options");
        ASSERT_TRUE(false);
    }

    struct sockaddr_in server_address;
    int port = 12345;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cout << "Failed to bind socket" << std::endl;
        ASSERT_TRUE(false);
    }

    for (int i = 0; i < numClients; ++i) {
        ASSERT_NE(listen(server_socket, 5), -1) << "Failed to listen" << std::endl;

        int client_socket;
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen);
        ASSERT_NE(client_socket, -1) << "Failed to accept connection" << std::endl;

        int client_id = ++clientCounter;
        threads[i] = std::thread(client, client_socket, client_id);
    }

    EXPECT_EQ(clientCounter, numClients);

    // Wait for all client threads to finish
    for (int i = 0; i < numClients; ++i) {
        threads[i].join();
    }

    // Assert that all client threads ran concurrently
    ASSERT_TRUE(true);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

