#include "ImgPack.h"
#include <json/json.h>
#include <fstream>

data::ImgPack::ImgPack()
{
	textureAtlas = new TextureAtlas(TEXTURE_ATLAS_MAX_WIDTH, TEXTURE_ATLAS_MAX_WIDTH);
}

void data::ImgPack::addImgObj(std::string name,const GdkPixbuf* srcImg,const def::IMG_SIZE size)
{	
	_imgs.push_back(ImgObj(name,size, textureAtlas->addAddImgs(srcImg, size))); // add img to the end of the vector //
}

void data::ImgPack::addImgObjFromJson(ImgObj imgobj)
{
	_imgs.push_back(imgobj);
}

std::list<data::ImgObj>& data::ImgPack::getImgVec()
{
	return _imgs;
}

void data::ImgPack::delImgObj(std::string name)
{
	// try to find ImgObj by name // O(n)
	std::list<data::ImgObj>::iterator iterator = find(name);
	// if founded //
	if (iterator != _imgs.end())
	{
		math::Vec2<int> ref = iterator->getRef(0);
		// erase the image O(1) //	
		_imgs.erase(iterator);
		// erase the image O(1) *new algorithm* //	
		textureAtlas->delImgObj(ref);				
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

void data::ImgPack::saveImgPackAsJsonFile()
{
    Json::Value jsonValue;

	for (auto it = _imgs.begin(); it != _imgs.end(); it++)
	{
		jsonValue[it->getName()]["size"] = it->getSizeAsString();
		for (int i = 0; i < it->getSizeAsInt(); i++)
		{			

			jsonValue[it->getName()][std::to_string(i) + "_ref_x"] = it->getRef(i).getX();
			jsonValue[it->getName()][std::to_string(i) + "_ref_y"] = it->getRef(i).getY();
		}
	}

	Json::StreamWriterBuilder builder;

	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";  
	std::unique_ptr<Json::StreamWriter> writer(
		builder.newStreamWriter());
	
	// Make a new JSON document for the configuration. Preserve original comments.
	
	
	std::ofstream ofs("resources//img_pack.json", std::ofstream::out);// file to read //
	writer->write(jsonValue, &ofs);
	
	ofs.close();
}

bool compare_nocase(const data::ImgObj& first, const data::ImgObj& second)
{
	if (first.getName() < second.getName())
		return false;
	return false;
}

void data::ImgPack::sort()
{
	_imgs.sort(compare_nocase);
}