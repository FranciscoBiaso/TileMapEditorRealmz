#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include "Thing.h"

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
		void addItem(data::Thing);

		/**
		 * @brief This methods remove a item by name.
		 */
		void removeItem(std::string name);
	};

}

