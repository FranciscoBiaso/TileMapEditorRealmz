#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <gdk/gdk.h>
#include "Thing.h"
#include "Vec3.h"

namespace scene {
	/*!
		Cylinder
		========

		This class represents a data structure to store things, it's a container.
	*/
	class Cylinder
	{
	private:
		std::vector<data::Thing> items;
		math::Vec3<int> coords;
	public:
		
		/**
		 * Constructor.
		 */
		Cylinder();

		/**
		 * @brief This methods returns item[index].
		 */
		const data::Thing& operator[](int index);

		/**
		 * @brief This methods returns top item.
		 */
		const data::Thing& getTopItem();

		/**
		 * @brief This methods add a new item into the structure then sort by layer.
		 */
		data::Thing addItem(data::Thing);

		/**
		 * @brief This methods remove a item by name.
		 */
		void removeItem(std::string name);

		/**
		 * @brief This methods cleans the cylinder.
		 */
		void cleans();

		/**
		 * @brief This methods sets cylinder coords.
		 */
		void setCoords(int x, int y, int z);

		/**
		 * @brief This methods gets cylinder coords.
		 */
		math::Vec3<int> getCoords();

		/**
		 * @brief This methods draws all things by layer.
		 */
		void draw(cairo_t* cr);

		/**
		 * @brief This method checks if the new Thing to be added already exists.
		 * @return true if already exists them swaps the vector item. false if the item doesn't exist.
		 */
		bool ifItemByLayerAlreadyExistsSwap(data::Thing& toAdd);
	};

}

