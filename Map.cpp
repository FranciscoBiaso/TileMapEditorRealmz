#include "Map.h"
#include "Cylinder.h"
#include <gtk/gtk.h>

scene::Map::Map(std::string name, int width, int height) : name(name),
	width(width), height(height)
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

data::Thing scene::Map::addThing(data::Thing newThing, int line, int col, int level)
{
	
	newThing.setName(std::to_string(_count_things));
	_count_things++;
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
}

void scene::Map::drawMap(cairo_t* cr)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			structure[0][width * y + x].draw(cr);
		}
	}
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
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			structure[0][width * y + x].removeItem(thingName);
		}
	}
}

int scene::Map::getCountThings()
{
	return _count_things;
}