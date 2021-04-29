#ifndef Client_hpp_HEADERGUARD
#define Client_hpp_HEADERGUARD

#include "CommunicationChannel.hpp"

#include <boost/asio.hpp>

#include <memory>
#include <functional>


class Client{
	public:
		using OnConnectHandler = std::function<void()>;
		using ResponseHandler = std::function<void(std::string)>;

		Client(boost::asio::io_service&, boost::asio::ip::tcp::endpoint);

		void connect();
		void request(std::string, ResponseHandler);
		
		void setOnConnectHandler(OnConnectHandler);

		boost::asio::ip::tcp::socket& getSocket();
		bool isConnected();
	private:
		void handleConnect(boost::system::error_code);

		std::shared_ptr<boost::asio::ip::tcp::socket> m_tcpSocket;
		boost::asio::ip::tcp::endpoint m_endpoint;
		std::shared_ptr<CommunicationChannel> m_communicationChannel;
		bool m_connected;

		OnConnectHandler m_onConnectHandler;
};


#endif //Client_hpp_HEADERGUARD
