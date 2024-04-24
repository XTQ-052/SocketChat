# SocketChat

## About the Project
SocketChat is a terminal-based chat application that uses socket programming to enable real-time text communication between users. It supports multiple clients, allowing for group chat functionality alongside private messaging.

## Getting Started
To use SocketChat, you need to first run the server to activate a listening port. Once the server is running, you can start as many clients as you wish to begin messaging.

### Prerequisites
Ensure you have a C++ compiler installed, such as g++, and your system supports socket programming.

### Running the Server
1. Open a terminal window.
2. Navigate to the directory containing the `server.cpp` file.
3. Compile the server code:
   ```bash
   g++ server.cpp -o server
4. Run the compiled server executable:
   ./server
   
### Running the Client
1. Open a new terminal window for each client.
2. Navigate to the directory containing the client.cpp file.
3. Compile the client code:
   g++ client.cpp -o client
4. Run the compiled client executable:
   ./client
   
### Features
* Real-Time Messaging: Instantly send and receive messages.
* Multiple Clients: Supports multiple users connected at the same time.
* Group Chat: Communicate with several users in a group chat setting.

### Contributing
Contributions are welcome! For major changes, please open an issue first to discuss what you would like to change.
