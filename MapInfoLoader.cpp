#include "MapInfoLoader.h"
#include <fstream>
#include <json/json.h>
#include <iostream>

void MapInfoLoader::load()
{
	std::ifstream ifs("resources//app.json");// file to read //
	Json::CharReaderBuilder rbuilder;	// reader //
	std::string errs; // to check errors //
	Json::Value jsonObj;
	Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //
	mapWidth = jsonObj["map_width"].asInt();
	mapHeight = jsonObj["map_height"].asInt();
	mapLevels = jsonObj["map_levels"].asInt();	
}

int MapInfoLoader::getMapWidth() { return mapWidth; }
int MapInfoLoader::getMapHeight() { return mapHeight; }
int MapInfoLoader::getMapLevels() { return mapLevels; }