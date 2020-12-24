#include "MapResources.h"
#include "AuxUI.h"
extern ui::AuxUI* gAuxUI;

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

void data::MapResources::delThing(std::string thingName, std::string keyThingType) {}

/**
 *  @brief This method deletes a Thing object from the stuffBook.
 *  @param thingName object to be deleted.
 */
void data::MapResources::delThing(std::string thingName)
{
	for (auto it = stuffBook.begin(); it != stuffBook.end(); it++)
	{
		auto result = it->second.find(thingName);
		if (result != it->second.end()) // we found a thing //
		{
			it->second.erase(thingName);
		}
	}
}

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
				std::string name = _class[j]["name"].asString(); // access name //
				layerOrder.insert(std::make_pair(name, layer));
				stuffBook.insert(std::pair<std::string, std::map<std::string, data::Thing>>(name, std::map<std::string, data::Thing >())); // fill our data //
			}
		}
		else{
			layerOrder.insert(std::make_pair(name, layer));
			stuffBook.insert(std::pair<std::string, std::map<std::string, data::Thing>>(name, std::map<std::string, data::Thing >())); // fill our data //
		}
	}
}


int data::MapResources::getLayerAsInt(const std::string name)
{
	return layerOrder[name];
}

void data::MapResources::saveStuffBook()
{
	//std::map<std::string, std::map<std::string, data::Thing>> stuffBook; // dicionary of dicionary (things) //
	Json::Value jsonValue;
	data::Thing thing;
	std::string thingName;
	for (auto it = stuffBook.begin(); it != stuffBook.end(); it++)
	{
		std::map sub_dicionary = it->second;
		for (auto j = sub_dicionary.begin(); j != sub_dicionary.end(); j++)
		{
			thing = j->second;
			thingName = j->first;
			jsonValue[thingName]["type"] = thing.getType();
			jsonValue[thingName]["img_ptr"] = thing.getImgObjPtr()->getName();
		}
	}

	Json::StreamWriterBuilder builder;

	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";
	std::unique_ptr<Json::StreamWriter> writer(
		builder.newStreamWriter());

	// Make a new JSON document for the configuration. Preserve original comments.


	std::ofstream ofs("resources//stuff_book.json", std::ofstream::out);// file to read //
	writer->write(jsonValue, &ofs);

	ofs.close();
}

void data::MapResources::loadStuffBookFromJson()
{	
	std::ifstream ifs("resources//stuff_book.json");// file to read //
	Json::CharReaderBuilder rbuilder;	// reader //
	std::string errs; // to check errors //
	Json::Value jsonObj;
	data::Thing thing;	
	Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //   
	if (!jsonObj.isNull()) // loading img pack //
	{
		gAuxUI->printMsg("Loading StuffBook from file!");
		
		for (auto it = jsonObj.begin(); it != jsonObj.end(); it++)
		{
			std::string name = it.name();
			std::string imgObjName = (*it)["img_ptr"].asString();
			thing.setName(name);
			thing.setType((*it)["type"].asString());
			auto iterator = getImgPack().find(imgObjName);
			if (iterator != getImgPack().getImgVec().end())
			{
				thing.setImgObjPtr(&(*iterator));
				addThing(thing);
			}
		}
		
	}
	else
	{
		gAuxUI->printMsg("No ImgPack data to be loaded!");
	}
}

int data::MapResources::getItemFromStuffBook(std::string itemName, data::Thing & t)
{
	for (auto it = stuffBook.begin(); it != stuffBook.end(); it++)
	{
		auto result = it->second.find(itemName);
		if (result != it->second.end()) // we found a thing //
		{	
			t =  result->second;
			return 1;
		}
	}
	return 0;
}

std::map<std::string, int>& data::MapResources::getLayerDic()
{
	return layerOrder;
}