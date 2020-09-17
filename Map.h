#pragma once

#include "Definitions.h"
#include <vector>
#include <string>
#include "Cylinder.h"
#include <gdk/gdk.h>
#include "Vec2.h"

namespace data { class Thing; }
class GLScence; 

namespace scene {

	/*!
		Map class
		=========

		This class represents internal data structure of the map.
	*/
	class Map
	{
	protected:
		GLScence* _glScence;
		std::string name;/**< name of the map */
		int width;/**< width */
		int height;/**< height */
		int levels;/**< total levels or floors */
		std::vector<std::vector<Cylinder>> structure; /**< internal structure (unity = cylinder) */
		int _count_things;/**< count things placed into the map - starts empty */

		/**
		 * @brief This method delets all thing with name = thingName from the map.
		 */
		void deletAllThings(std::string thingName);
	public:
		/**
		 * Constructor.
		 */
		Map(std::string name, int width, int height, int levels);

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
		data::Thing addThing(data::Thing newThing, int line, int col, int level);

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

		/**
		 * @brief Draw map strcuture.
		 */
		void drawMap(cairo_t* cr, math::Vec2<int> camera_position, int width, int height,bool draw_borders);

		/**
		 * @brief Get count things placed into the map.
		 */
		int getCountThings();

		void setGlScene(GLScence*);

		void saveInternalMap();
		void loadInternalMapFromJson();
	};
}


