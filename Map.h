#pragma once

#include "Definitions.h"
#include <vector>
#include <string>
#include <array>

namespace data { class Thing; }

namespace scene {

	class Cylinder;
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
		std::array< std::vector<Cylinder>, MAP_COUNT_LEVELS> structure;
	public:
		/**
		 * Constructor.
		 */
		Map(std::string name, int width, int height);

		/**
		 * @brief This method gets position from index [0,1].
		 */
		Cylinder & at(int line, int col, int level)
		{
			if ((line < 0 || line >= height) || (col < 0 || col >= width) || (level < 0 || level >= MAP_COUNT_LEVELS))
				throw def::ReturnMsg::ARRAY_OUT_OF_RANGE;
			return structure[level][width * line + col];
		}

		/**
		 * @brief This method adds a Thing into the map position [x,y,z];
		 */
		void addThing(data::Thing newThing, int x, int y,int z);

		/**
		 * @brief This method removes a Thing from the map position [x,y,z] by name;
		 */
		void removeThing(std::string name, int x, int y, int z);
	};
}


