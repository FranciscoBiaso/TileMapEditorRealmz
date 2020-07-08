#pragma once

#include "Definitions.h"
#include <vector>
#include <string>
#include "Cylinder.h"
#include <gdk/gdk.h>

namespace data { class Thing; }

namespace scene {

	/*!
		Map class
		=========

		This class represents internal data structure of the map.
	*/
	class Map
	{
	private:
		std::string name;
		int width;
		int height;
		int levels;
		std::vector<std::vector<Cylinder>> structure;		
	protected:
		/**
		 * @brief This method delets all thing with name = thingName from the map.
		 */
		void deletAllThings(std::string thingName);
	public:
		/**
		 * Constructor.
		 */
		Map(std::string name, int width, int height);

		/**
		 * @brief This method gets Cylinder from position (x,y,z).
		 */
		Cylinder & at(int line, int col, int level)
		{
			if ((line < 0 || line >= height) || (col < 0 || col >= width) || (level < 0 || level >= MAP_COUNT_LEVELS))
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return structure[level][width * line + col];
		}

		/**
		 * @brief This method adds a Thing into the map position [line,col,level].
		 */
		void addThing(data::Thing newThing, int line, int col, int level);

		/**
		 * @brief This method remove all things from mouse position.
		 */
		void cleansCylinder(int line, int col, int level);

		/**
		 * @brief This method removes a Thing from the map position [x,y,z] by name.
		 */
		void removeThing(std::string name, int x, int y, int z);

		/**
		 * @brief This method gets map width.
		 */
		int getWidth() const;

		/**
		 * @brief This method gets map height.
		 */
		int getHeight() const;

		void drawMap(cairo_t* cr);
	};
}


