#include "ImgPack.h"


data::ImgPack::ImgPack()
{
	textureAtlas = new TextureAtlas(TEXTURE_ATLAS_MAX_WIDTH, TEXTURE_ATLAS_MAX_WIDTH);
}

void data::ImgPack::addImgObj(std::string name,const GdkPixbuf* srcImg,const def::IMG_SIZE size)
{	
	_imgs.push_back(ImgObj(name,size, textureAtlas->addAddImgs(srcImg, size))); // add img to the end of the vector //
}

std::list<data::ImgObj>& data::ImgPack::getImgVec()
{
	return _imgs;
}

void data::ImgPack::delImgObj(std::string name)
{
	// try to find ImgObj by name // O(n)
	std::list<data::ImgObj>::iterator it = find(name);
	// if founded //
	if (it != _imgs.end())
	{
		// remove imgs from texture atlas O(n) //
		textureAtlas->delImgObj(it, _imgs.end());
		// erase the image O(1) //
		_imgs.erase(it);
	}
}

std::list<data::ImgObj>::iterator data::ImgPack::find(std::string imgName)
{
	return std::find_if(_imgs.begin(), _imgs.end(), [&, imgName](data::ImgObj& img) {
		if (img.getName() == imgName)
			return true;
		return false;
		});	
}

data::TextureAtlas* data::ImgPack::getTextureAtlas() const
{
	return textureAtlas;
}
