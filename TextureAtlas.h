#pragma once
#include <gdk/gdk.h>
#include "Definitions.h"

class ImgObj;

namespace data {
	/*!
		TextureAtlas class
		==================

		This class represents a data strcuture to store the images. 

		The iamges will be stored in a vector as an array of rows and columns.
		Each unit in the array will store an image (row,column) position with WIDTH and height equal REALMZ_GRID_SIZE.
	 */
	class TextureAtlas
	{
	private:
		static GdkPixbuf* pixelBuf;  // Atlas into memory //

		int rowCursor;
		int colCursor;
	
		void resetCursor();
		void rightShiftCursor();
	public:
		/**
		 * constructor.
		 * 
		 * @param width count 32x32 imgs rows.
		 * @param height count 32x32 imgs cols.
		 */
		TextureAtlas(int width, int height);

		// ------- | ------- //
		//         |         //
		//    1ª   |    2º   //
		//         |         //
		// ------- | ------- // graphical representation of srcImg pixelbuf
		//         |         //
		//    4º   |    3º   //
		//         |         //
		// ------- | ------- //
		/**
		 * @brief This methods fill the texture atlas with real images copying data from widget pixelbuf region to atlas pixelbuf.
		 * 
		 * srcImg region should has exactly 64x64 size.
	     * if size == 32x32 then copy center of srcImg (REALMZ_GRID_SIZE/2,REALMZ_GRID_SIZE/2).
	     * if size == 32x64 then copy entire y and center x and the order to add is from left to right.
	     * if size == 64x32 then copy entire x and center y and the order to add is from top to bot.
	     * if size == 64x64 then copy all region and the order to add is clockwise.
		 * 
		 * @param srcImg pixelbuf that contains the data to be copied.
		 * @param size of ImgObj.
		 */
		void addImgObj(GdkPixbuf* srcImg, def::IMG_SIZE size);

	};
}

