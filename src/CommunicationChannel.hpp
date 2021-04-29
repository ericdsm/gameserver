#ifndef CommunicationChannel_hpp_HEADERGUARD
#define CommunicationChannel_hpp_HEADERGUARD

#include <boost/asio.hpp>

#include <memory>
#include <string>


class CommunicationChannel{
	public:
		static const std::string MESSAGE_SEPARATOR;

		using OnReadHandler = std::function<void(std::string)>;
		using OnWriteHandler = std::function<void()>;

		CommunicationChannel(std::shared_ptr<boost::asio::ip::tcp::socket>);

		void read();
		void write(std::string);

		void setOnReadHandler(OnReadHandler);
		void setOnWriteHandler(OnWriteHandler);

	private:
		void readHandler(boost::system::error_code, std::shared_ptr<boost::asio::streambuf>);
		void writeHandler(boost::system::error_code, std::shared_ptr<boost::asio::streambuf>);

		std::shared_ptr<boost::asio::ip::tcp::socket> m_tcpSocket;

		OnReadHandler m_onReadHandler;
		OnWriteHandler m_onWriteHandler;

};


#endif //CommunicationChannel_hpp_HEADERGUARD
