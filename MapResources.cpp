#include "MapResources.h"

// load map structure from json file //
data::MapResources::MapResources(){
	createStuffBookFromJson();
}

// add new thing into the std::map stuffBook if it doesn't exits //
void data::MapResources::addThing(Thing thing){
	auto it = stuffBook.find(thing.getType());
	if (it != stuffBook.end()) // we found the thing type //
	{
		it->second.insert(std::pair<std::string, Thing>(thing.getName(), thing));
	}
}

// remove thing by name from std map if it exits //
void  data::MapResources::delThing(std::string thingName, std::string keyThingType) {

}

void data::MapResources::createStuffBookFromJson()
{
	std::ifstream ifs("resources//thing_types.json");
	Json::CharReaderBuilder rbuilder;
	
	std::string errs;
	Json::parseFromStream(rbuilder, ifs, &thingTypesJson, &errs);
	const Json::Value& types = thingTypesJson["types"]; 
	for (auto i = 0; i < types.size(); i++) {
		std::string name = types[i]["name"].asString();
		int layer = types[i]["layer"].asUInt();
		const Json::Value& _class = types[i]["class"];
		if (_class.size() != 0) {
			for (auto j = 0; j < _class.size(); j++) {
				stuffBook.insert(std::pair<std::string, std::map<std::string, Thing>>(_class[j]["name"].asString(), std::map<std::string, Thing >()));
			}
		}
		else{
			stuffBook.insert(std::pair<std::string, std::map<std::string, Thing>>(name, std::map<std::string, Thing >()));
		}
	}
}