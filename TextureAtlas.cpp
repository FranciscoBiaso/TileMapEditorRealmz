#include "TextureAtlas.h"
GdkPixbuf* data::TextureAtlas::pixelBuf = nullptr;
data::TextureAtlas::TextureAtlas(int width, int height)
{
	if(pixelBuf == nullptr)
		pixelBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width * REALMZ_GRID_SIZE, height * REALMZ_GRID_SIZE);
	if (pixelBuf == NULL)
		throw def::ReturnMsg::NOT_ENOUGH_MEMORY;
	resetCursor();
}

void data::TextureAtlas::resetCursor()
{
	rowCursor = colCursor = 0;
}

void data::TextureAtlas::rightShiftCursor()
{
	if (colCursor == TEXTURE_ATLAS_MAX_WIDTH)
		rowCursor++;
	colCursor %= TEXTURE_ATLAS_MAX_WIDTH;
}

void data::TextureAtlas::addImgObj(GdkPixbuf* srcImg, def::IMG_SIZE size)
{
	if(gdk_pixbuf_get_width(srcImg) != 64 || gdk_pixbuf_get_height(srcImg) != 64)
		throw def::ReturnMsg::PIXELBUF_INVALID_SIZE;

	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32:
	{
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
	}
		break;
	case def::IMG_SIZE::IMG_SIZE_32X64: 
	{
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
	}
		break;
	case def::IMG_SIZE::IMG_SIZE_64X32:
	{
		gdk_pixbuf_copy_area(srcImg, 0, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
	}
		break;
	case def::IMG_SIZE::IMG_SIZE_64X64: 
	{
		gdk_pixbuf_copy_area(srcImg, 0, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, rowCursor * REALMZ_GRID_SIZE, colCursor * REALMZ_GRID_SIZE);
		rightShiftCursor();
	}
		break;
	default: 
		break;
	}
}

void data::TextureAtlas::delImgObj(data::ImgObj imgObj)
{
	switch (imgObj.getSize())
	{
	case def::IMG_SIZE::IMG_SIZE_32X32:
	{

	}
		break;

	}
}
