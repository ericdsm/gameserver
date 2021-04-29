#include "Character.hpp"

#include "Map.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <sstream>
#include <iostream>



Character::Character(std::string name):
m_name(name),
m_level(0),
m_experience(0)
{}

bool Character::readFromXml(boost::filesystem::path characterPath){
	boost::property_tree::ptree ptree;

	boost::property_tree::read_xml(characterPath.c_str(), ptree);

	m_name = ptree.get<std::string>("character.name");
	m_level = ptree.get<unsigned int>("character.level");
	m_experience = ptree.get<unsigned int>("character.experience");

	m_position.x = ptree.get<int>("character.position.<xmlattr>.x");
	m_position.y = ptree.get<int>("character.position.<xmlattr>.y");
	m_position.z = ptree.get<int>("character.position.<xmlattr>.z");

	return true;
}

bool Character::writeToXml(boost::filesystem::path characterPath){
	boost::property_tree::ptree ptree;

	ptree.put("character.name", m_name);
	ptree.put("character.level", m_level);
	ptree.put("character.experience", m_experience);

	ptree.put("character.position.<xmlattr>.x", m_position.x);
	ptree.put("character.position.<xmlattr>.y", m_position.y);
	ptree.put("character.position.<xmlattr>.z", m_position.z);

	boost::property_tree::write_xml(characterPath.c_str(), ptree);

	return true;
}

bool Character::readFromDatabase(mongo::DBClientConnection &dbConnection, std::string name){
	mongo::BSONObj character = dbConnection.query("mud.characters", BSON("name" << name))->next();
	std::cout << dbConnection.getLastErrorDetailed().toString() << std::endl;

	m_name = character.getStringField("name");
	m_level = character.getIntField("level");
	m_experience = character.getIntField("experience");

	
	mongo::BSONObj position = character.getObjectField("position");

	m_position.x = position.getIntField("x");
	m_position.y = position.getIntField("x");
	m_position.z = position.getIntField("x");

	return true;
}

bool Character::writeToDatabase(mongo::DBClientConnection &dbConnection){
	mongo::BSONObj character = BSON(
		"name" << m_name <<
		"level" << m_level <<
		"experience" << m_experience <<
		"position" << BSON(
			"x" << m_position.x <<
			"y" << m_position.y <<
			"z" << m_position.z
		)
	);

	dbConnection.insert("mud.characters", character);
	std::cout << dbConnection.getLastErrorDetailed().toString() << std::endl;

	return true;
}

std::string Character::getLookStr(const Map &map) const{
	std::stringstream ss;
	const Map::Area &currentArea = map.m_areas.at(m_position);

	ss << "[" << currentArea.name << "]" << std::endl << currentArea.description;

	return ss.str();
}

void Character::addMessage(const std::string &s){
	m_messages += s;
	m_messages += "\n";
}

std::string Character::getMessages() const{
	return m_messages.substr(0, m_messages.size()-1);
}

void Character::clearMessages(){
	m_messages = "";
}

Position Character::getPosition() const{
	return m_position;
}

const std::string& Character::getName() const{
	return m_name;
}
