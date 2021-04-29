#include "Map.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <iostream>


bool Map::readFromXml(boost::filesystem::path mapPath){
	boost::property_tree::ptree ptree;

	boost::property_tree::read_xml(mapPath.c_str(), ptree);

	for(boost::property_tree::ptree::value_type &v : ptree.get_child("map")){
		Area a;
		a.name = v.second.get<std::string>("name");
		a.description = v.second.get<std::string>("description");
		Position p;
		p.x = v.second.get<int>("position.<xmlattr>.x");
		p.y = v.second.get<int>("position.<xmlattr>.y");
		p.z = v.second.get<int>("position.<xmlattr>.z");
		//std::cout << a.name << p.x << std::endl;
		m_areas.insert(std::pair<Position, Area>(p, a));
	}

	return true;
}
