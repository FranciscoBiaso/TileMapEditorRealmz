#pragma once
#include <string>
#include <vector>
#include <map>
#include "Definitions.h"
#include "Vec2.h"

namespace data {
	/*!
		ImgObj class
		============

		This class represents an image object that can contain n image references where n is equal to 1, 2 or 4 in the following sizes: 32x32,32x64,64x32 or 64x64.
	 */
	class ImgObj
	{
	private:
		std::string name;
		math::Vec2<int> imgs[IMGOBJ_MAX_IMGS];
		def::IMG_SIZE size;
	public:
		/**
		 * constructor.
		 */
		ImgObj(std::string name, def::IMG_SIZE size): name(name), size(size){}

		/**
		 * constructor. 
		 * @param size internal img size (32x32, 32x64, 64x32 or 64x64).
		 */
		ImgObj(std::string name, def::IMG_SIZE size, const std::vector<math::Vec2<int>> references);

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a def::IMG_SIZE.
		 */
		def::IMG_SIZE getSize() const;

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a std::string.
		 */
		std::string getSizeAsString() const;

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a int.
		 */
		int getSizeAsInt() const;

		/**
		 * @brief This method gets std::string name;
		 */
		const std::string getName() const { return name; }

		/**
		 * @brief This method sets ImgObj references as a const std::vector<math::Vec2<int>>;
		 */
		void setImgRefs(const std::vector<math::Vec2<int>> references);

		/**
		 * @brief This method gets math::Vec2<int> from index i.
		 */
	    math::Vec2<int> getRef(int i) const { return imgs[i]; }

		/**
		 * @brief This method sets math::Vec2<int> ref from index i.
		 */
		void setImgRef(int i, math::Vec2<int> v) { imgs[i].setX(v.getX()); imgs[i].setY(v.getY()); }
	};

}

