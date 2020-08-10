#include "Map.h"
#include "Cylinder.h"
#include <gtk/gtk.h>
#include "GLScence.h"

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
#include <iostream>
#include "Vec2.h"
#include <glm/glm.hpp>
#include "Thing.h"
#include "ImgObj.h"

data::Thing scene::Map::addThing(data::Thing newThing, int line, int col, int level)
{	
	newThing.setName(std::to_string(_count_things));
	_count_things++;
	
	math::Vec2<int> ref = newThing.getImgObjPtr()->getRef(0);
	glm::vec2 glmRef(ref.getX(), ref.getY());
	Quad & quad = _glScence->getQuad(line * getWidth() + col);
	quad.setTextCoord(glmRef);
	quad.setColor(glm::vec4(1, 1, 1, 1));

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

void scene::Map::setGlScene(GLScence* gl)
{
	_glScence = gl;
}