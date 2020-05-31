#pragma once
#include <map>
#include <string>
#include "Thing.h"
#include <fstream>
#include <json/json.h>
#include <iostream>

namespace data {
	/*!
		MAP EDITOR RESOURCES
		====================

		This class contains the resources that will be used by the software.
	 */
	class MapResources{

	private:
		std::map<std::string, std::map<std::string, Thing>> stuffBook; // dicionary of dicionary (things) //
		Json::Value thingTypesJson; // used to manipulate json files //

		/**
		 *  @brief This method fills stuffBook data loading from json file.
		 */
		void createStuffBookFromJson(); 
		void loadImgBook() {} 
	public:
		/**
		 *  constructor
		 */
		MapResources();

		/**
		 *  @brief This method adds a Thing object to the stuffBook.
		 *  @param Thing Object to be added.
		 */
		void addThing(Thing thingObj);

		/**
		 *  @brief This method deletes a Thing object from the stuffBook.
		 *  @param thingName object to be deleted.
		 *  @param thingType class of this object.
		 */
		void delThing(std::string thingName, std::string thingType);  

		/**
		 *  @brief This method gets the stuffBook.
		 */
		std::map<std::string, std::map<std::string, Thing>>& getStuffBook() { return stuffBook; }

		/**
		 *  @brief This method gets the a json object.
		 */
		const Json::Value getThingTypesJson() { return thingTypesJson; }
	};
};

