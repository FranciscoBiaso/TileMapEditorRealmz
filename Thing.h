#pragma once
#include <string>


class Thing
{
public:
	Thing(std::string name, std::string strType);

	std::string getName() { return name; }
	std::string getType() { return type; }

	void setName(std::string name) { this->name = name; }
	void setType(std::string type) { this->type = type; }

private:
	std::string name;
	std::string type;
	std::string img;

};

