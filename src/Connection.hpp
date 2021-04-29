#ifndef Connection_hpp_HEADERGUARD
#define Connection_hpp_HEADERGUARD

#include "CommunicationChannel.hpp"

#include <boost/asio.hpp>

#include <functional>
#include <string>


class Connection{
	public:
		using OnMessageHandler = std::function<void(CommunicationChannel&, std::string)>;

		Connection(boost::asio::io_service&);

		void start();

		void setOnMessageHandler(OnMessageHandler);

		boost::asio::ip::tcp::socket& getSocket();
		const std::shared_ptr<CommunicationChannel> getCommunicationChannelPtr();
	private:
		void readHandler(std::string);
		void writeHandler();

		std::shared_ptr<boost::asio::ip::tcp::socket> m_tcpSocket;
		std::shared_ptr<CommunicationChannel> m_communicationChannel;

		OnMessageHandler m_onMessageHandler;
};


#endif //Connection_hpp_HEADERGUARD
