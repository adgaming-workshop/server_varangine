#include "server.h"

int main(){
	Server::getInstance()->data_storage.createObject("rock");
	Server::getInstance()->runServer();
	return 0;
}

