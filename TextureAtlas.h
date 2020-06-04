#pragma once
#include <gdk/gdk.h>
#include "Definitions.h"
#include "ImgObj.h"
#include "Vec2.h"

#define AT_ROW 0
#define AT_COL 1

namespace data {
	/*!
		TextureAtlas class
		==================

		This class represents a data structure to store ImgObj data. 

		The images will be stored in a vector as an array of rows and columns.
		Each unit in the array will store an image (row,column) position with width and height equal REALMZ_GRID_SIZE (32x32).
	 */
	class TextureAtlas
	{
	private:
		static GdkPixbuf* pixelBuf;  // Atlas into memory //
		math::Vec2<int> cursor;
		data::ImgObj* last4ImgsPtr[IMGOBJ_MAX_IMGS]; // we need last 4 imgs to keep delet with O(1) time //
	
		void resetCursor();
		void rightShiftCursor();
		void leftShiftCursor();
	public:
		TextureAtlas(){}
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
		 * @return images references added into texture atlas.
		 */
		const std::vector<math::Vec2<int>> addAddImgs(const GdkPixbuf* srcImg, const def::IMG_SIZE size);

		/**
		 * @brief This methods erases the texture atlas releasing data of atlas pixelbuf.
		 */
		void delImgObj(const ImgObj *);

		
		void rightShiftPtrs();
		void leftShiftPtrs();
		void setFirstPtr(ImgObj*);

		GdkPixbuf* getPixelbuf() const { return pixelBuf; }
	};

}

