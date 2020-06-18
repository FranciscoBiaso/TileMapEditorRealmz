#include "Map.h"
#include "Cylinder.h"

scene::Map::Map(std::string name, int width, int height) : name(name),
	width(width), height(height)
{
	for (int level = 0; level < levels; level++)
	{
		this->structure[level].resize(width * height);
	}	
}

void scene::Map::addThing(data::Thing newThing, int x, int y, int z)
{
	this->structure[z][height * y + x].addItem(newThing);
}

void scene::Map::removeThing(std::string name, int x, int y, int z)
{
	this->structure[z][height * y + x].removeItem(name);
}