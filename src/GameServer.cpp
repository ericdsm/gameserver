#include "GameServer.hpp"

#include <iostream>


GameServer::GameServer(boost::asio::io_service &ioService, unsigned short int port):
m_port(port),
m_ip(boost::asio::ip::address_v4::any()),
m_endpoint(m_ip, m_port),
m_server(new Server(ioService, m_endpoint)),
m_characterDir("./data/characters/"),
m_mapPath("./data/Map.xml")
{
	m_server->setOnConnectHandler(std::bind(&GameServer::connectHandler, this, std::placeholders::_1));
	m_server->setOnMessageHandler(std::bind(&GameServer::messageHandler, this, std::placeholders::_1, std::placeholders::_2));

	m_map.readFromXml(m_mapPath);
}

void GameServer::start(){
	std::cout << "Starting game server... " << std::flush;
	m_server->start();
	std::cout << "Done." << std::endl;
}

void GameServer::connectHandler(Connection &conn){
	std::string a = conn.getSocket().remote_endpoint().address().to_string();
	std::cout << "Client connected from " << a << std::endl;

	createSession(conn.getCommunicationChannelPtr());
}

void GameServer::createSession(std::shared_ptr<CommunicationChannel> channel){
	m_sessions.emplace(std::make_pair<CommunicationChannel*, Session>(channel.get(), {*this, channel}));
}

void GameServer::messageHandler(CommunicationChannel &channel, std::string req){
	std::cout << "Received \"" << req << "\"" << std::endl;

	m_sessions.at(&channel).handleRequest(req);
}

boost::filesystem::path GameServer::getCharacterDir(){
	return m_characterDir;
}

Map& GameServer::getMap(){
	return m_map;
}
