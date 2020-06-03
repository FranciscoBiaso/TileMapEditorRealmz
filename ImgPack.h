#pragma once

#include "ImgObj.h"
#include "TextureAtlas.h"
#include <map>
#include <string>
#include <gdk/gdk.h>
namespace data {
	class ImgPack
	{
	private:
		std::map<std::string, data::ImgObj> map;
		data::TextureAtlas textureAtlas;

		/**
		 * @brief This method is used to increase memory when needed.		 
		 */
		void resizeTextureAtlas();

	public:
		/**
		 * @brief This method add <name,ImgObj> into std::map and fill textureAtlas with srcImg pixelBuf.
		 * @param name map key of the ImgObj to be added.
		 * @param srcImg pixel buffer region with pixels to fill textureAtlas.
		 * @param size ImgSize to be copied.
		 */
		void addImgObj(std::string name, GdkPixbuf* srcImg, def::IMG_SIZE size);

		/**
		 * @brief This method delete <name,ImgObj> from std::map and images from textureAtlas.
		 * @param name map key of the ImgObj to be deleted.
		 */
		void delImgObj(std::string name);

		/**
		 * @brief This method gets ImgObj * with complexity O(log(n)). 
		 * Used when resources will be loaded.
		 * \return ImgObj *.
		 */
		const data::ImgObj* getImgObjPtr();
	};
};
