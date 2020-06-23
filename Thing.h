#pragma once
#include <string>
#include <gdk/gdk.h>

namespace data { class ImgObj; }
namespace scene { class Cylinder; }

namespace data {
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
		 *  copy constructor by pointer.
		 */
		Thing(Thing *);

		/**
		 *  copy constructor by ref.
		 */
		Thing(const Thing &);

		/**
		 *  @brief This method gets name.
		 */
		std::string getName() { return name; }

		/**
		 *  @brief This method gets type.
		 */
		const std::string getType() { return type; }

		/**
		 *  @brief This method gets imgObj_ptr (data::ImgObj*).
		 */
		const data::ImgObj* getImgObjPtr() { return imgObj_ptr; }

		/**
		 *  @brief This method sets name [same as layer].
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

		/**
		 *  @brief This method draws a Thing Obj.
		 */
		void draw(cairo_t * cr);

		/**
		 *  @brief This method sets Cylinder.
		 */
		void setCylinder(scene::Cylinder*);

		/**
		 *  @brief This method gets Cylinder.
		 */
		const scene::Cylinder* getCylinder();

		/**
		 *  @brief This method overloads assigned operator.
		 */
		data::Thing& operator=(const data::Thing& v2);
	private:
		std::string name; 
		std::string type;
		data::ImgObj* imgObj_ptr;
		static GdkPixbuf* pixelbuf; // pixelbuff used to draw all things //
		scene::Cylinder* parent; // parent cylinder of this Thing //
	};
}