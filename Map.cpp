#include "Map.h"
#include "Cylinder.h"
#include <gtk/gtk.h>
#include "GLScence.h"
#include "json/json.h"
#include <fstream>
#include "AuxUI.h"

extern ui::AuxUI* gAuxUI;
extern data::MapResources* gResources;

scene::Map::Map(std::string name, int width, int height, int levels) : name(name),
	width(width), height(height), levels(levels)
{
	_count_things = 0;
	levels = MAP_COUNT_LEVELS;
	structure.resize(levels);
	for (int level = 0; level < levels; level++)
	{
		this->structure[level].resize(width * height);
		for (int row = 0; row < height; row++)
		{
			for (int col = 0; col < width; col++)
			{
				this->structure[level][width * row + col].setCoords(col, row, level);
			}
		}
	}	
}
#include <iostream>
#include "Vec2.h"
#include <glm/glm.hpp>
#include "Thing.h"
#include "ImgObj.h"

data::Thing scene::Map::addThing(data::Thing newThing, int line, int col, int level)
{	
	newThing.setName(std::to_string(_count_things));
	_count_things++;
	// GET THING REF //
	math::Vec2<int> ref = newThing.getImgObjPtr()->getRef(0);
	glm::vec2 glmRef(ref.getX(), ref.getY());

	// GET QUAD FROM RENDER STRUCTURE  //
	GLRect& quad = _glScence->getQuad((line * getWidth() + col) + level * getWidth() * getHeight());
	// SET VALUES //
	quad.setTextCoord(glmRef);


	scene::Cylinder& cylinder = at(line, col, level);
	// we only reste color if it has light //
	if(cylinder.hasLight())
		quad.setColor(glm::vec4(1, 1, 1, 1));

	// add into internal structure
	return this->structure[level][width * line + col].addItem(newThing);	
}

void scene::Map::cleansCylinder( int line, int col, int level)
{
	this->structure[level][width * line + col].cleans();
}

void scene::Map::removeThing(std::string name, int line, int col, int level)
{
	this->structure[level][width * line + col].removeItem(name);
	_count_things--; 
	_glScence->getQuad(line * getWidth() + col).reset_textcoord(-1);
}

void scene::Map::drawMap(cairo_t* cr, math::Vec2<int> camera_position, int widthTiles, int heightTiles, bool draw_borders)
{
	/*
	int x_start_position = camera_position.getX() - widthTiles /2;
	if (x_start_position < 0)
	{
		x_start_position = 0;
		if (draw_borders) // DRAW BORDER LEFT //
		{
			cairo_rectangle(cr, coords.getX() * REALMZ_GRID_SIZE, coords.getY() * REALMZ_GRID_SIZE, 32, 32);
		}
	}
	int x_end_position = camera_position.getX() + widthTiles / 2;
	if (x_end_position > width)
		x_end_position = width;
	
	int y_start_position = camera_position.getY() - height / 2;
	if (y_start_position < 0)
		y_start_position = 0;
	int y_end_position = camera_position.getY() + height / 2;
	if (y_end_position > height)
		y_end_position = height;

	for (int line = y_start_position; line < y_end_position; line++)
	{
		for (int col = x_start_position; col < x_end_position; col++)
		{
			structure[0][width * line + col].draw(cr);	
		}
	}

	cairo_stroke(cr);
	*/
}

int scene::Map::getWidth() const
{
	return width;
}

int scene::Map::getHeight() const
{
	return height;
}

void scene::Map::deletAllThings(std::string thingName)
{
	for (int level = 0; level < levels; level++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				structure[level][width * y + x].removeItem(thingName);
			}
		}
	}
}

int scene::Map::getCountThings()
{
	return _count_things;
}

void scene::Map::setGlScene(GLScence* gl)
{
	_glScence = gl;
}

void scene::Map::saveInternalMap()
{
	Json::Value root;
	Json::Value jsonArrayCylinders(Json::arrayValue);
	root["0_count_things"] = _count_things;
	//root["0_levels"] = levels;
	//root["0_width"] = width;
	//root["0_height"] = height;
	root["0_name"] = name;
	
	Json::Value cylinderJson;
	for (int z = 0; z < levels; z++)
	{
		int floor = levels - 1 - z;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{

				Json::Value jsonArrayItems(Json::arrayValue);
				scene::Cylinder & cylinder = structure[floor][width * y + x];
				auto items = structure[floor][width * y + x].getItems();

				for (int i = 0; i < items.size(); i++)
				{
					jsonArrayItems.append(items[i].getStuffBookRefName());
				}
				cylinderJson["items"] = jsonArrayItems;
				cylinderJson["y"] = y;
				cylinderJson["x"] = x;
				cylinderJson["z"] = floor;
				cylinderJson["has_light"] = cylinder.hasLight();
				jsonArrayCylinders.append(cylinderJson);
			}
		}
	}

	root["map_units"] = jsonArrayCylinders;

	Json::StreamWriterBuilder builder;

	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";
	std::unique_ptr<Json::StreamWriter> writer(
		builder.newStreamWriter());

	// Make a new JSON document for the configuration. Preserve original comments.


	std::ofstream ofs("resources//map_things.json", std::ofstream::out);// file to read //
	writer->write(root, &ofs);

	ofs.close();
}

void scene::Map::loadInternalMapFromJson()
{
	std::string mapName = name;
	std::ifstream ifs("resources//map_things.json");// file to read //
	Json::CharReaderBuilder rbuilder;	// reader //
	std::string errs; // to check errors //
	Json::Value jsonObj;
	data::Thing thing;
	Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //   
	if (!jsonObj.isNull()) // loading img pack //
	{
		if (jsonObj["0_name"].asString() != mapName)
			return;

		gAuxUI->printMsg("Loading Map from file!");
		Json::Value map_units = jsonObj["map_units"];
		
		for (int i =0;i< map_units.size(); i++)
		{
				
			Json::Value cylinder = map_units[i];
			int x = cylinder["x"].asInt();
			int y = cylinder["y"].asInt();
			int z = cylinder["z"].asInt();
			bool hasLight = cylinder["has_light"].asBool();

			Json::Value itens = map_units[i]["items"];

			for (int j = 0; j < itens.size(); j++)
			{
				std::string stuffBookItemName = itens[j].asString();
				data::Thing aux;

				if (gResources->getItemFromStuffBook(stuffBookItemName, aux) != 0)
				{			
					aux.setStuffBookRefName(stuffBookItemName);
					addThing(aux, y, x, z);
					Cylinder& cylinder = at(y, x, z);
					cylinder.setLight(hasLight);
					if (!hasLight)
						_glScence->removeLightCylindergMapUI((y * getWidth() + x) + z * getWidth() * getHeight());
				}
			}
			
		}	
	}
	else
	{
		gAuxUI->printMsg("No ImgPack data to be loaded!");
	}
}