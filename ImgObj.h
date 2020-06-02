#pragma once
#include <string>
#include <vector>
#include "Definitions.h"

namespace data {
	/**
	 * struct ImgRef.
	 */
	typedef struct{
		int line; /**< line represents y position into the texture atlas images. */
		int col;  /**< col represents x position into the texture atlas images. */
	} ImgRef;
		
	class ImgObj
	{
	private:
		std::string name; // name of the img obj //
		IMG_SIZE size; // size of this image 32x32, 32x64, etc //		
		std::vector<ImgRef> imgs; // references to real images //

		/**
		 * @brief This method sets name of ImgObj.
		 * @param size of the ImgObj.
		 */
		void setSize(IMG_SIZE size);
	public:
		/**
		 * constructor. 
		 * @param size internal img size (32x32, 32x64, 64x32 or 64x64).
		 */
		ImgObj(IMG_SIZE size = IMG_SIZE::IMG_SIZE_32X32);

		/**
		 * @brief This method sets name of ImgObj.
		 * @param name of the ImgObj.
		 */
		void setName(std::string name);

		/**
		 * @brief This method sets name of ImgObj.
		 * @param index of imgs vector.
		 * @param ref of imgs vector[index].
		 */
		void setImg(int index, ImgRef ref);

		/**
		 * @brief This method gets name of ImgObj.
		 */
		std::string getName() const { return name; }
		
		/**
		 * @brief This method gets IMG_SIZE of ImgObj.
		 */
		IMG_SIZE getSize() const { return size; }

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a std::string.
		 */
		std::string getSizeAsString() const;

		/**
		 * @brief This method gets IMG_SIZE of ImgObj as a int.
		 */
		int getSizeAsInt();

		 /**
		 * @brief This method reallocates the imgs vector to the specified size.
		 * @param size to be reallocated.
		 */
		void resize(IMG_SIZE size = IMG_SIZE::IMG_SIZE_32X32);
	};

}

