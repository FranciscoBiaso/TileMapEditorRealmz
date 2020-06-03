#include "ImgObj.h"

data::ImgObj::ImgObj(def::IMG_SIZE size)
{
	resize(); // sets size(32x32) and alocate imgs vector //
}

void data::ImgObj::setSize(def::IMG_SIZE size)
{
	this->size = size;
}

void data::ImgObj::setImg(int index, ImgRef ref)
{
	// code prevent error //
	if (index < 0 || index > imgs.size() - 1)
		return;// in future we will throw ; //
	imgs[index] = ref;
}

std::string data::ImgObj::getSizeAsString() const
{
	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32: return "32x32";
		break;
	case def::IMG_SIZE::IMG_SIZE_32X64: return "32x64";
		break;
	case def::IMG_SIZE::IMG_SIZE_64X32: return "64x32";
		break;
	case def::IMG_SIZE::IMG_SIZE_64X64: return "64x64";
		break;
	default: return "getSizeAsString() unknown type!";
		break;
	}
}

int data::ImgObj::getSizeAsInt()
{
	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32: return 1;
		break;
	case def::IMG_SIZE::IMG_SIZE_32X64: // return 2 //
	case def::IMG_SIZE::IMG_SIZE_64X32: return 2;
		break;
	case def::IMG_SIZE::IMG_SIZE_64X64: return 4;
		break;
	default: return 1;
		break;
	}	
}

void data::ImgObj::resize(def::IMG_SIZE size)
{
	setSize(size);
	imgs.resize(getSizeAsInt());
}
