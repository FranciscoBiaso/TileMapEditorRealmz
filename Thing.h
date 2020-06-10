#pragma once
#include <string>
#include "ImgObj.h"

namespace TME {
	/*!
		Thing obj
		=========

		This class represents the elements that can fill the map.
	 */
	class Thing
	{
	public:
		/**
		 *  constructor
		 */
		Thing(std::string name, std::string strType);

		/**
		 *  @brief This method gets name.
		 */
		std::string getName() { return name; }

		/**
		 *  @brief This method gets type.
		 */
		std::string getType() { return type; }

		/**
		 *  @brief This method sets name.
		 */
		void setName(std::string name) { this->name = name; }

		/**
		 *  @brief This method sets type.
		 */
		void setType(std::string type) { this->type = type; }

	private:
		std::string name; 
		std::string type;
		std::string img;
		data::ImgObj* imgObj_ptr;
	};
}