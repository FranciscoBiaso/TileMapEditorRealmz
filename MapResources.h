#pragma once
#include <map>
#include <string>
#include <fstream>
#include <json/json.h>
#include <iostream>

#include "Thing.h"
#include "ImgPack.h"

namespace data {
	/*!
		MAP EDITOR RESOURCES
		====================

		This class contains the resources that will be used by the software.
	 */
	class MapResources{

	private:
		std::map<std::string, std::map<std::string, data::Thing>> stuffBook; // dicionary of dicionary (things) //
		Json::Value thingTypesJson; // used to manipulate json files //
		ImgPack imgPack;
		std::map<std::string, int> layerOrder; // dicionary of layers loading json //
		/**
		 *  @brief This method fills stuffBook data loading from json file.
		 */
		void createStuffBookFromJson();

	public:
		/**
		 *  constructor
		 */
		MapResources();

		/**
		 *  @brief This method adds a Thing object to the stuffBook.
		 *  @param Thing Object to be added.
		 */
		void addThing(data::Thing thingObj);

		/**
		 *  @brief This method deletes a Thing object from the stuffBook.
		 *  @param thingName object to be deleted.
		 *  @param thingType class of this object.
		 */
		void delThing(std::string thingName, std::string thingType); 


		/**
		 *  @brief This method deletes a Thing object from the stuffBook.
		 *  @param thingName object to be deleted.
		 */
		void delThing(std::string thingName);

		/**
		 *  @brief This method gets the stuffBook.
		 */
		std::map<std::string, std::map<std::string, data::Thing>>& getStuffBook() { return stuffBook; }

		/**
		 *  @brief This method gets the a json object.
		 */
		const Json::Value getThingTypesJson() { return thingTypesJson; }

		ImgPack& getImgPack() { return imgPack; }

		/**
		 *  @brief This method gets layer as integer.
		 */
		int getLayerAsInt(const std::string name);

		void saveStuffBook();
		void loadStuffBookFromJson();

		int getItemFromStuffBook(std::string itemName, data::Thing &);
		std::map<std::string, int>& getLayerDic();

	};
}

