#include "TextureAtlas.h"

GdkPixbuf* data::TextureAtlas::pixelBuf = nullptr;

data::TextureAtlas::TextureAtlas(int width, int height)
{
	last4ImgsPtr[0] = nullptr;
	last4ImgsPtr[1] = nullptr;
	last4ImgsPtr[2] = nullptr;
	last4ImgsPtr[3] = nullptr;

	if(pixelBuf == nullptr)
		pixelBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width * REALMZ_GRID_SIZE, height * REALMZ_GRID_SIZE);
	if (pixelBuf == NULL)
		throw def::ReturnMsg::NOT_ENOUGH_MEMORY;
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
		if (cursor(AT_ROW) != 0)
			cursor(AT_ROW) = cursor(AT_ROW) - 1;
		else if (cursor(AT_COL) > 0)
			cursor(AT_COL) = cursor(AT_COL) - 1;
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

void data::TextureAtlas::rightShiftPtrs()
{
	last4ImgsPtr[1] = last4ImgsPtr[0];
	last4ImgsPtr[2] = last4ImgsPtr[1];
	last4ImgsPtr[3] = last4ImgsPtr[2];
}

void data::TextureAtlas::leftShiftPtrs()
{
	last4ImgsPtr[0] = last4ImgsPtr[1];
	last4ImgsPtr[1] = last4ImgsPtr[2];
	last4ImgsPtr[2] = last4ImgsPtr[3];
	last4ImgsPtr[3] = nullptr;
}

void data::TextureAtlas::setFirstPtr(ImgObj* imgObjPtr)
{
	last4ImgsPtr[0] = imgObjPtr;
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
void data::TextureAtlas::delImgObj(const data::ImgObj * imgObj)
{
	int countImgs = imgObj->getSizeAsInt();
	int ptrCounter = 0;
	while(countImgs > 0)
	{
		if (last4ImgsPtr[ptrCounter] != nullptr)
		{
			for (int i = 0; i < last4ImgsPtr[ptrCounter]->getSizeAsInt(); i++)
			{
				leftShiftCursor();
				math::Vec2 ref = imgObj->getRef(countImgs - 1);
				gdk_pixbuf_copy_area(pixelBuf,
					cursor[AT_ROW] * REALMZ_GRID_SIZE, cursor[AT_COL] * REALMZ_GRID_SIZE,
					REALMZ_GRID_SIZE, REALMZ_GRID_SIZE,
					pixelBuf,
					ref[AT_ROW] * REALMZ_GRID_SIZE, ref[AT_COL] * REALMZ_GRID_SIZE);
				countImgs--;
				leftShiftPtrs();
			}
		}
		ptrCounter++;
	}
}
