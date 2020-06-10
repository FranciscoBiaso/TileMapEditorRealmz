#include "ImgPack.h"


data::ImgPack::ImgPack()
{
	textureAtlas = new TextureAtlas(TEXTURE_ATLAS_MAX_WIDTH, TEXTURE_ATLAS_MAX_WIDTH);
}

void data::ImgPack::addImgObj(std::string name,const GdkPixbuf* srcImg,const def::IMG_SIZE size)
{	
	std::vector<math::Vec2<int>> refs = textureAtlas->addAddImgs(srcImg, size); // add img into texture atlas and grab reference //
	imgVec.push_back(ImgObj(name,size,refs)); // add img to the end of the vector //
}

void data::ImgPack::delImgObj(std::string name)
{
	// try to find ImgObj by name // O(n)
	std::vector<data::ImgObj>::iterator it = find(name);
	// if founded //
	if (it != imgVec.end())
	{
		// remove imgs from texture atlas O(n) //
		textureAtlas->delImgObj(it, imgVec.end());
		// erase the image O(1) //
		imgVec.erase(it);
	}
}

std::vector<data::ImgObj>::iterator data::ImgPack::find(std::string imgName)
{
	return std::find_if(imgVec.begin(), imgVec.end(), [&, imgName](data::ImgObj& img) {
		if (img.getName() == imgName)
			return true;
		return false;
		});	
}

data::TextureAtlas* data::ImgPack::getTextureAtlas() const
{
	return textureAtlas;
}
