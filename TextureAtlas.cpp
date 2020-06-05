#include "TextureAtlas.h"
#include "MapResources.h"
#include <iostream>
using namespace std;
GdkPixbuf* data::TextureAtlas::pixelBuf = nullptr;
GdkPixbuf* data::TextureAtlas::pixelBufClean32x32 = nullptr;

extern data::MapResources* gResources;

data::TextureAtlas::TextureAtlas(int width, int height)
{
	if(pixelBuf == nullptr)
		pixelBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width * REALMZ_GRID_SIZE, height * REALMZ_GRID_SIZE);
	if (pixelBuf == NULL)
		throw def::ReturnMsg::NOT_ENOUGH_MEMORY;

	pixelBufClean32x32 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
	gdk_pixbuf_fill(pixelBufClean32x32, 0x22222222); // clean buffer //

	resetCursor();
}

void data::TextureAtlas::resetCursor()
{
	cursor.setXY(0,0);
}

void data::TextureAtlas::rightShiftCursor()
{
	if (cursor(AT_COL) == (TEXTURE_ATLAS_MAX_WIDTH-1))
		cursor(AT_ROW) = cursor(AT_ROW) + 1;
	cursor(AT_COL) = (cursor(AT_COL) + 1) % TEXTURE_ATLAS_MAX_WIDTH;	
}

void data::TextureAtlas::leftShiftCursor()
{
	if (cursor(AT_COL) == 0)
	{
		if (cursor(AT_ROW) > 0)
			cursor(AT_COL) = TEXTURE_ATLAS_MAX_WIDTH;
	}
	else
		cursor(AT_COL) = cursor(AT_COL) - 1;
}

const std::vector<math::Vec2<int>> data::TextureAtlas::addAddImgs(const GdkPixbuf* srcImg, const def::IMG_SIZE size)
{
	std::vector<math::Vec2<int>> refs;
	if (gdk_pixbuf_get_width(srcImg) != 64 || gdk_pixbuf_get_height(srcImg) != 64)
		throw def::ReturnMsg::PIXELBUF_INVALID_SIZE;
	
	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32:
	{
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();		
	}
	break;
	case def::IMG_SIZE::IMG_SIZE_32X64:
	{
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
	}
	break;
	case def::IMG_SIZE::IMG_SIZE_64X32:
	{
		gdk_pixbuf_copy_area(srcImg, 0, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
	}
	break;
	case def::IMG_SIZE::IMG_SIZE_64X64:
	{
		gdk_pixbuf_copy_area(srcImg, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
	}
	break;
	default:
		break;
	}
	return refs;
}

void data::TextureAtlas::setIterator(ImgObj* lastImgAdd)
{	
	this->lastImgAdd = lastImgAdd;
}

/*
N equal nullptr
~ equal new data
× equal empty
I ImgObj
| cursor

#addAddImgs# (A)
0 1 2 3 4 5 0 1 2 3
× × × × × × N N N N // before algorithm
~ × × × × × I N N N // 1º case, add ImgObj 32x32
~ ~ × × × × I I N N // 2º case, add ImgObj 32x32
~ ~ ~ × × × I I I N // 3º case, add ImgObj 32x32

#delImgObj#  (A.1)
0 1 2 3 4 5 0 1 2 3
~ ~ ~ × × × I I I N // before algorithm
	  |
~ ~ x × × × I I N N // delete ImgObj 1 (32x32)
	|

#addAddImgs# (B)
0 1 2 3 4 5 0 1 2 3
× × × × × × N N N N // before algorithm
~ × × × × × I N N N // 1º case, add ImgObj 32x32
~ ~ ~ × × × I I N N // 2º case, add ImgObj 64x32
~ ~ ~ ~ × × I I I N // 3º case, add ImgObj 32x32

#delImgObj#  (B.1)
0 1 2 3 4 5 0 1 2 3
~ ~ ~ ~ × × I I I N // before algorithm
		|
~ ~ ~ ~ × × I I I N // delete ImgObj 1 (64x32)
	  |
*/

/**
 * tests
 * 
 * [1º test, only 1 ImgObj]: sizes: 32x32, 32x64, 64x32, 64x64. (test ok)
 * [2º test, only 2+ ImgObj]: sizes: ([1]:32x32,[2]:32x32), ([1]:32x32,[2]:32x64), ([1]:32x32,[2]:64x32), etc. 
 * Deleting 1ª -> 2ª and 2ª -> 1ª
 * 
 * \param imgObj
 */
void data::TextureAtlas::delImgObj(ImgObj* imgObjToDelete)
{
	int countImgsToDel = imgObjToDelete->getSizeAsInt();

	while(countImgsToDel > 0 && lastImgAdd != nullptr)
	{
		for (int i = 0; i < lastImgAdd->getSizeAsInt(); i++)
		{
			leftShiftCursor();

			math::Vec2 ref = imgObjToDelete->getRef(countImgsToDel - 1);
			// transfer data //
			gdk_pixbuf_copy_area(pixelBuf,
				cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE,
				REALMZ_GRID_SIZE, REALMZ_GRID_SIZE,
				pixelBuf,
				ref[AT_COL] * REALMZ_GRID_SIZE, ref[AT_ROW] * REALMZ_GRID_SIZE);
			std::cout << "cursor[AT_COL]:" << cursor[AT_COL] << "cursor[AT_ROW]:" << cursor[AT_ROW] << endl;
			std::cout << "ref[AT_COL]:" << ref[AT_COL] << "ref[AT_ROW]:" << ref[AT_ROW] << endl;			

			// clean data //
			gdk_pixbuf_copy_area(pixelBufClean32x32,
				0, 0,
				REALMZ_GRID_SIZE, REALMZ_GRID_SIZE,
				pixelBuf,
				lastImgAdd->getRef(i)[AT_COL] * REALMZ_GRID_SIZE, lastImgAdd->getRef(i)[AT_ROW] * REALMZ_GRID_SIZE);
			
			//update those values//
			lastImgAdd->setImgRef(i, ref);
			countImgsToDel--;

			if (countImgsToDel == 0)
			{			
				break;
			}
		}
		ImgObj* tmp = lastImgAdd;
		if (lastImgAdd->getPrevious() != nullptr)
		{
			lastImgAdd->getPrevious()->setNext(nullptr);
		}
		lastImgAdd = lastImgAdd->getPrevious();
		if (imgObjToDelete->getPrevious() != nullptr && imgObjToDelete->getNext() != nullptr)
		{
			//std::cout << "case a" << endl;
			imgObjToDelete->setNext(tmp);
			tmp->setPrevious(imgObjToDelete);
			imgObjToDelete->getNext()->setPrevious(tmp);
			tmp->setNext(imgObjToDelete->getNext());
		}
		else if (imgObjToDelete->getPrevious() == nullptr && imgObjToDelete->getNext() != nullptr)
		{
			//std::cout << "case b" << endl;
			imgObjToDelete->getNext()->setPrevious(tmp);
			tmp->setNext(imgObjToDelete->getNext());
			tmp->setPrevious(nullptr);
		}
		else if (imgObjToDelete->getPrevious() != nullptr && imgObjToDelete->getNext() == nullptr)
		{
			//std::cout << "case c" << endl;
			imgObjToDelete->getPrevious()->setNext(tmp);
			tmp->setPrevious(imgObjToDelete->getPrevious());
			tmp->setNext(nullptr);
		}
		else if (imgObjToDelete->getPrevious() == nullptr && imgObjToDelete->getNext() == nullptr)
		{
			//std::cout << "case d" << endl;
			tmp->setPrevious(nullptr);
			tmp->setNext(nullptr);
		}		
	}	
}
