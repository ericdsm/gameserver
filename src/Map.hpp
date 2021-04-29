#ifndef Map_hpp_HEADERGUARD
#define Map_hpp_HEADERGUARD

#include "Position.hpp"

#include <boost/filesystem.hpp>

#include <map>


struct Map{
	struct Area{
		std::string name;
		std::string description;
	};
	bool readFromXml(boost::filesystem::path);

	std::map<Position, Area> m_areas;
};


#endif //Map_hpp_HEADERGUARD
