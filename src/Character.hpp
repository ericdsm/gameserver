#ifndef Character_hpp_HEADERGUARD
#define Character_hpp_HEADERGUARD

#include "Inventory.hpp"
#include "Position.hpp"

#include <boost/filesystem.hpp>
#include <string>

#include "mongo/client/dbclient.h"


struct Map;

class Character{
	public:
		Character() = default;
		Character(std::string name);

		bool readFromXml(boost::filesystem::path);
		bool writeToXml(boost::filesystem::path);

		bool readFromDatabase(mongo::DBClientConnection &dbConnection, std::string name);
		bool writeToDatabase(mongo::DBClientConnection &dbConnection);

		void addMessage(const std::string &s);
		std::string getMessages() const;
		void clearMessages();


		const std::string& getName() const;
		std::string getLookStr(const Map&) const;
		Position getPosition() const;
	private:
		std::string m_name;
		//Class m_class;
		unsigned int m_level;
		unsigned int m_experience;

		Position m_position;

		Inventory m_inventory;

		std::string m_messages;
};


#endif //Character_hpp_HEADERGUARD
