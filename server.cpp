#include "server.h"
#include "game_object.h"
#include <vector>

Server::Server(){};
Server::Server(const Server&){};
Server& Server::operator=(const Server&){};

Server* Server::getInstance(){
	if (!instance){
		instance = new Server;
		return instance;
	}
	else 
		return instance;
}

void Server::deleteInstance(){
	delete instance;
	instance = 0;
}

Server* Server::instance = 0;

sf::Packet& operator<< (sf::Packet& PACKET, const DataStorage& data_storage){
	for (auto obj: data_storage.getAllObjects()){
		PACKET << obj->x << obj->y;  
		PACKET << obj->velocity.x << obj->velocity.y;  
		PACKET << obj->accel.x << obj->accel.y;  
	}
	return PACKET;
}

sf::Packet& operator>> (sf::Packet& PACKET, DataStorage& data_storage){
	for (auto obj: data_storage.getAllObjects()){
		PACKET >> obj->x >> obj->y;  
		PACKET >> obj->velocity.x >> obj->velocity.y;  
		PACKET >> obj->accel.x >> obj->accel.y;  
	}
	return PACKET;
}

void Server::makeListener(){
	if (this->listener.listen(53000) != sf::Socket::Done){
                std::cout << "Can't bind the port" << std::endl;
                return;
        }
}

bool Server::connect(){
	if (this->listener.accept(this->socket) != sf::Socket::Done) {
        	std::cout << "Can not accept connection" << std::endl;
            	return false;
        }
        return true;
}

bool Server::extractData(){
	if(this->Data >> this->data_storage)
		return true;
	return false;	
}

void Server::manage(){
	for(;;){
                sf::Socket::Status status = this->socket.receive(this->Data);
            	if (status == sf::Socket::Done){
                	if(!(this->extractData()))
                       		std::cout << "Data extracted unsuccessfully" << std::endl;
            	}
            	if (status == sf::Socket::Disconnected){
                	//std::cout << "Client disconnected" << std::endl;
                	break;
            	}
        }
}

void Server::registrate(std::string client_name){
	this->clients.push_back(client_name);
}

void Server::unregistrate(std::string client_name){
	auto beg = this->clients.begin();
	auto end = this->clients.end();
	for(auto it = beg; it != end; it++){
		if(*it == client_name){
			clients.erase(it);
			return;
		}
	}
}

bool Server::isRegistrated(std::string client_name){
	for (auto name: this->clients)
		if(name == client_name)
			return true;
	return false;
}

void Server::runServer(){
	this->makeListener();
	for(;;){
		if (this->connect()){
			this->manage();
			this->send();
		}
		else
			break;
	}
}

void Server::makePacket(){
	this->updates << this->data_storage; 
}

void Server::send(){
	this->makePacket();
        if(this->socket.connect("127.0.0.1", 53000) != sf::Socket::Done)
                std::cout << "Connnection failed" << std::endl;
        sf::Socket::Status status = this->socket.send(this->updates);
      	if (status != sf::Socket::Done)
        	std::cout << "Message has not been sent!" << std::endl;
        this->socket.disconnect();
}



