#pragma once
#include <string>

namespace data { class ImgObj; }

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
		Thing(std::string name = "-", std::string strType = "-");

		/**
		 *  @brief This method gets name.
		 */
		std::string getName() { return name; }

		/**
		 *  @brief This method gets type.
		 */
		std::string getType() { return type; }

		/**
		 *  @brief This method gets imgObj_ptr (data::ImgObj*).
		 */
		const data::ImgObj* getImgObjPtr() { return imgObj_ptr; }

		/**
		 *  @brief This method sets name.
		 */
		void setName(std::string name) { this->name = name; }

		/**
		 *  @brief This method sets type.
		 */
		void setType(std::string type) { this->type = type; }

		/**
		 *  @brief This method sets ImgObj.
		 */
		void setImgObjPtr(data::ImgObj* imgObj_ptr) { this->imgObj_ptr = imgObj_ptr; }

	private:
		std::string name; 
		std::string type;		
		data::ImgObj* imgObj_ptr;
	};
}