#include "AppLoaderSettings.h"
#include <fstream>
#include <json/json.h>
#include <iostream>

void AppLoaderSettings::load()
{
	std::ifstream ifs("resources//app.json");// file to read //
	Json::CharReaderBuilder rbuilder;	// reader //
	std::string errs; // to check errors //
	Json::Value jsonObj;
	Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //
	mapWidth = jsonObj["map_width"].asInt();
	mapHeight = jsonObj["map_height"].asInt();
	mapToLoad = jsonObj["map_to_load"].asString();
	mapLevels = jsonObj["map_levels"].asInt();
}

int AppLoaderSettings::getMapWidth() { return mapWidth; }
int AppLoaderSettings::getMapHeight() { return mapHeight; }
int AppLoaderSettings::getMapLevels() { return mapLevels; }
std::string AppLoaderSettings::getMapNameToLoad() {return mapToLoad;}