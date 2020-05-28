#pragma once
#include <map>
#include <string>
#include "Thing.h"
#include <fstream>
#include <json/json.h>
#include <iostream>

namespace data {

	class MapResources{

	private:
		std::map<std::string, std::map<std::string, Thing>> stuffBook; // dicionary of dicionary (things) //
		Json::Value thingTypesJson; // used to manipulate json files //

		void createStuffBookFromJson(); // load thing types from json file //
		void loadImgBook() {} 
	public:
		MapResources();

		void addThing(Thing thingObj); // add thing into stuffbook //
		void delThing(std::string thingName, std::string keyThingType);  //delet thing from stuffbook //

		/* get */
		std::map<std::string, std::map<std::string, Thing>>& getStuffBook() { return stuffBook; }
		const Json::Value getThingTypesJson() { return thingTypesJson; }
	};
};

