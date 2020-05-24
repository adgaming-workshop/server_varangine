#ifndef __SERVER_H__
#define __SERVER_H__

#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "physics.h"
#include "graphics.h"
#include "scripts.h"
#include "data_storage.h"


class Server{
private:
	static Server* instance; //Singleton
	Server();
	Server& operator=(const Server&);
	Server(const Server&);
	void deleteInstance();

	sf::TcpSocket socket; //Получение данных 
	sf::TcpListener listener;
	sf::Packet Data;
	sf::Packet updates;
	void makeListener();
	bool connect();
	void manage(); //В этой функции обработка данных
	bool extractData();
	void makePacket();
	void send();

	std::vector<std::string> clients;
	void registrate(std::string client_name);
	void unregistrate(std::string client_name);
	bool isRegistrated(std::string client_name);

public:
	static Server* getInstance();

	PhysicsController physics_controller;
	ScriptController script_controller;
	RenderController render_controller;
	DataStorage data_storage;

	void runServer(); //Работа всего сервера
};
	

#endif
