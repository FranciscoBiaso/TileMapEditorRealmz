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
	mapToLoad = jsonObj["map_to_load"].asString();	
	editorType = jsonObj["editor_type"].asString();
}

std::string AppLoaderSettings::getMapNameToLoad() {return mapToLoad;}
std::string AppLoaderSettings::getEditorType() { return editorType; }