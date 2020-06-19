#include "Map.h"
#include "Cylinder.h"

scene::Map::Map(std::string name, int width, int height) : name(name),
	width(width), height(height)
{
	levels = MAP_COUNT_LEVELS;
	structure.resize(levels);
	for (int level = 0; level < levels; level++)
	{
		this->structure[level].resize(width * height);
		for (int row = 0; row <= height; row++)
		{
			for (int col = 0; col <= width; col++)
			{
				this->structure[level][height * row + col].setCoords(col, row, level);
			}
		}
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

void scene::Map::drawMap(cairo_t* cr)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			structure[0][height * y + x].draw(cr);
		}
	}
}