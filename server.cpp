#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <algorithm>

struct Groups{
	std::string groupName;
	std::vector<int> users;

	Groups(const std::string& name) : groupName(name) {}

	Groups(const std::string& name, std::vector<int>& user) : groupName(name), users(user){}
};

void handleClient(int index, int clientSocket, std::vector<Groups>& groupsClientSockets, int clientNumber, std::vector<Groups>& groups){
	char buffer[1024];
	while(int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)){
		buffer[bytesRead] = '\0';
		std::cout << clientNumber << " client: " << buffer << std::endl;

		for(int otherclients : groupsClientSockets[index].users){
			if(otherclients != clientSocket){
				std::string message = std::to_string(clientNumber) + " : " + buffer;
				if(send(otherclients, message.c_str(), message.length(), 0) == -1) std::cerr << "Error sending message." << std::endl;
			}
		}

		memset(buffer, 0, sizeof(buffer));
	}
}

bool searchGroup(int& index, std::vector<Groups>& groups, std::string groupName) {
	for(int i=0; i<groups.size(); i++){
		if(groups[i].groupName == groupName){
			index = i;
			return true;
		}
	}
	return false;
}

int main(){
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //AF_INET=IpV4 //SOCKSTREAM=TCP //0=no special option is used
	sockaddr_in serverAddr = {AF_INET, htons(2248), INADDR_ANY}; //sockaddr_in=structure used to hold the port number //INADDR_ANY=IP address that the server will listen to

	//to bind a socket to a specific network address and port number
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(serverSocket, 20); //20=max client

	std::cout<<"Server listening..."<<std::endl;

	std::vector<Groups> groups;
	std::vector<Groups> groupsClientSockets;

	int clientNumber=1;
	int index = -1;

	while(true) {
		int clientSocket = accept(serverSocket, 0, 0); //accept and create new client socket

		//0=a pointer to a sockaddr structure that contains the address information of the client connection
		//If you don't need the client's address, you can pass NULL for this parameter

		//0=an integer specifying the size of the sockaddr structure
		//Since you're not retrieving the client's address information, specifying the size is unnecessary. When you're not collecting address information, the size is passed as 0

		if(clientSocket == -1) { // When the accept function fails, it typically returns the value -1
			std::cerr << "Error accepting connection." << std::endl;
			continue; // skip to the next iteration
		}

		std::cout << clientNumber << " client connected." << std::endl;

		char groupName_char[20];
		recv(clientSocket, groupName_char, sizeof(groupName_char), 0);
		std::string groupName(groupName_char);

		if(searchGroup(index, groups, groupName)){
			groups[index].users.push_back(clientNumber);
			groupsClientSockets[index].users.push_back(clientSocket);
			std::cout << clientNumber << " client connected to group " << groups[index].groupName << std::endl;
		}
		else{
			++index;
			groups.push_back(Groups(groupName));
			groups.back().users.push_back(clientNumber);

			groupsClientSockets.push_back(Groups(groupName));
			groupsClientSockets.back().users.push_back(clientSocket);

			std::cout << groupName << " group created by " << clientNumber << std::endl;
		}

		std::thread clientThread(handleClient, index, clientSocket, std::ref(groupsClientSockets), clientNumber, std::ref(groups));
		clientThread.detach();
		clientNumber++;
	}
	return 0;
}