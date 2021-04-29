#include "Connection.hpp"


Connection::Connection(boost::asio::io_service &ioService):
m_tcpSocket(new boost::asio::ip::tcp::socket(ioService)),
m_communicationChannel(new CommunicationChannel(m_tcpSocket))
{
	m_communicationChannel->setOnReadHandler(std::bind(&Connection::readHandler, this, std::placeholders::_1));
	m_communicationChannel->setOnWriteHandler(std::bind(&Connection::writeHandler, this));
}

void Connection::start(){
	m_communicationChannel->read();
}

void Connection::setOnMessageHandler(OnMessageHandler h){
	m_onMessageHandler = h;
}

boost::asio::ip::tcp::socket& Connection::getSocket(){
	return *m_tcpSocket;
}

const std::shared_ptr<CommunicationChannel> Connection::getCommunicationChannelPtr(){
	return m_communicationChannel;
}

void Connection::readHandler(std::string request){
	if(m_onMessageHandler){
		m_onMessageHandler(*m_communicationChannel, request);
	}
}

void Connection::writeHandler(){
	m_communicationChannel->read();
}
