#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

void clearLastLine() {
    std::cout << "\x1b[2K";  // Clear the line
}

void recvMessage(int clientSocket){
	char buffer[1024];
	while(int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)){
		buffer[bytesRead] = '\0';
		clearLastLine();
		std::cout << "\r" << buffer << std::endl;
		
		std::cout << "You: ";
        std::cout.flush();

		memset(buffer, 0, sizeof(buffer));
	}
}

int main(){
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr = {AF_INET, htons(2248), {}};

	if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) { // This function converts the provided IPv4 address from a specific address format to the format expected by the sin_addr field of the sockaddr_in structure
		std::cerr << "Invalid adress." << std::endl;
		return 1;
	}

	if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
		std::cerr << "Error connection to server." << std::endl;
		return 1;
	}

	std::cout << "Connected to the server." <<std::endl;

	std::cout << "Enter group name to join: ";

	char groupName[20];
	std::cin.getline(groupName, sizeof(groupName));
	if (send(clientSocket, groupName, sizeof(groupName), 0) == -1)
		std::cerr << "Error sending group name." << std::endl;

	std::thread recvMessageThread(recvMessage, clientSocket);

	recvMessageThread.detach();

	std::cout << "Enter a message or type 'exit' to quit" << std::endl;

	char message[1024];
	while(true){
		std::cout << "You: ";
		std::cin.getline(message, sizeof(message));

		if(strcmp(message, "exit") == 0) break; //strcmp=string compare //If the strings are equal, strcmp returns 0

		if(send(clientSocket, message, strlen(message), 0) == -1) { //If the operation fails, the send function usually returns -1.
			std::cerr << "Error sending message." << std::endl;
			break;
		}
	}

	close(clientSocket);

	return 0;
}