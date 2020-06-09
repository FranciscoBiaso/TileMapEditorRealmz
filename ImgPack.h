#pragma once

#include "ImgObj.h"
#include "TextureAtlas.h"
#include <string>
#include <gdk/gdk.h>


namespace data {
	/*!
		ImgPack class data structure
		============================

		This class represents a data structure capable of storing ImgObj objects and also managing memory sharing of real images (32x32, 32x64,64x32,64x64).
	 */
	class ImgPack
	{
	private:
		std::vector<data::ImgObj> imgVec;
		data::TextureAtlas* textureAtlas;
		/**
		 * @brief This method is used to increase memory when needed.		 
		 */
		void resizeTextureAtlas();
	public:
		/**
		 * constructor.
		 */
		ImgPack();
		/**
		 * @brief This method add ImgObj into this data structure.
		 * @param name key of the ImgObj to be added.
		 * @param srcImg pixel buffer region with pixels to fill textureAtlas.
		 * @param size ImgSize to be copied (32x32, 32x64, 64x32, 64x64).
		 */
		void addImgObj(std::string name, const GdkPixbuf* srcImg, const def::IMG_SIZE size);

		/**
		 * @brief This method delete ImgObj from this data strcture.
		 * @param name key of the ImgObj to be deleted.
		 */
		void delImgObj(std::string name);

		/**
		 *  @brief This method gets data::TextureAtlas*.
		 */
		data::TextureAtlas* getTextureAtlas() const;

		/**
		 *  @brief This method gets vector<data::ImgObj>&.
		 */
		std::vector<data::ImgObj>& getMap() { return imgVec; }

		/**
		 *  @brief This method gets count of ImgObj.
		 */
		int getCountImgs() { return imgVec.size(); }
	};
}
