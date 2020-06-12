#include "MapResources.h"

data::MapResources::MapResources()
{
	createStuffBookFromJson();
}

void data::MapResources::addThing(data::Thing thing){
	auto it = stuffBook.find(thing.getType());
	if (it != stuffBook.end()) // we found the thing type //
	{
		it->second.insert(std::pair<std::string, data::Thing>(thing.getName(), thing));
	}
}

void  data::MapResources::delThing(std::string thingName, std::string keyThingType) {}

void data::MapResources::createStuffBookFromJson()
{
	std::ifstream ifs("resources//thing_types.json");// file to read //
	Json::CharReaderBuilder rbuilder;	// reader //
	std::string errs; // to check errors //
	Json::parseFromStream(rbuilder, ifs, &thingTypesJson, &errs); // parser //
	const Json::Value& types = thingTypesJson["types"];  // access types //
	for (auto i = 0; i < types.size(); i++) { // iterate through them [types] // 
		std::string name = types[i]["name"].asString(); // access name //
		int layer = types[i]["layer"].asUInt(); // access layer //
		const Json::Value& _class = types[i]["class"]; // access class //
		if (_class.size() != 0) { // check if classes is bigger than 0 //
			for (auto j = 0; j < _class.size(); j++) { // iterate through them [classes] // 
				stuffBook.insert(std::pair<std::string, std::map<std::string, data::Thing>>(_class[j]["name"].asString(), std::map<std::string, data::Thing >())); // fill our data //
			}
		}
		else{
			stuffBook.insert(std::pair<std::string, std::map<std::string, data::Thing>>(name, std::map<std::string, data::Thing >())); // fill our data //
		}
	}
}