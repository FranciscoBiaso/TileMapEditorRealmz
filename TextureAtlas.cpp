#include "TextureAtlas.h"
#include "MapResources.h"

GdkPixbuf* data::TextureAtlas::pixelBuf = nullptr;
GdkPixbuf* data::TextureAtlas::pixelBufClean32x32 = nullptr;

extern data::MapResources* gResources;

data::TextureAtlas::TextureAtlas(int width, int height)
{
	if (pixelBuf == nullptr)
	{
		pixelBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, (TEXTURE_ATLAS_MAX_WIDTH +1) * REALMZ_GRID_SIZE, (TEXTURE_ATLAS_MAX_WIDTH +1) * REALMZ_GRID_SIZE);
		gdk_pixbuf_fill(pixelBuf, 0x55555544); // clean buffer //
	}
	if (pixelBuf == NULL)
		throw def::ReturnMsg::NOT_ENOUGH_MEMORY;

	pixelBufClean32x32 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE + 1, REALMZ_GRID_SIZE + 1);
	gdk_pixbuf_fill(pixelBufClean32x32, 0x22222200); // clean buffer //

	resetCursor();
}

void data::TextureAtlas::resetCursor()
{
	cursor.setXY(0,0);
}

void data::TextureAtlas::rightShiftCursor()
{
	if (cursor.getX() == (TEXTURE_ATLAS_MAX_WIDTH-1))
	{
		cursor.setX(0);
		cursor.setY(cursor.getY() + 1);
	}
	else
		cursor.setX((cursor.getX() + 1) % TEXTURE_ATLAS_MAX_WIDTH);
}

void data::TextureAtlas::leftShiftCursor()
{
	if (cursor.getX() == 0)
	{
		if (cursor.getY() > 0)
		{
			cursor.setY(cursor.getY() - 1);
			cursor.setX(TEXTURE_ATLAS_MAX_WIDTH - 1);
		}
	}
	else
		cursor.setX(cursor.getX() - 1);
}

const std::vector<math::Vec2<int>> data::TextureAtlas::addAddImgs(const GdkPixbuf* srcImg, const def::IMG_SIZE size)
{
	std::vector<math::Vec2<int>> refs;// get all references that were created //
	if (gdk_pixbuf_get_width(srcImg) != 64 || gdk_pixbuf_get_height(srcImg) != 64) // we do need continue if srcImg != (64x64) //
		throw def::ReturnMsg::PIXELBUF_INVALID_SIZE;
	
	// add images at the cursor position //
	switch (size)
	{
	case def::IMG_SIZE::IMG_SIZE_32X32:
	{
		// fill pixel with 32x32//
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();		
	}
	break;
	case def::IMG_SIZE::IMG_SIZE_32X64:
	{
		// fill pixel with 2x 32x32//
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, 0, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
	}
	break;
	case def::IMG_SIZE::IMG_SIZE_64X32:
	{
		// fill pixel with 2x 32x32//
		gdk_pixbuf_copy_area(srcImg, 0, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
		gdk_pixbuf_copy_area(srcImg, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE / 2, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, pixelBuf, cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
		refs.push_back(cursor); rightShiftCursor();
	}
	break;
	case def::IMG_SIZE::IMG_SIZE_64X64:
	{
		// fill pixel with 4x 32x32//
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

void data::TextureAtlas::delImgObj(std::vector<data::ImgObj>::iterator it, std::vector<data::ImgObj>::iterator end)
{
	int countShifts = it->getSizeAsInt();// get delete img size //
	math::Vec2<int> startPos = it->getRef(0);// reset startPosition cursor //
	math::Vec2<int> shiftFront = it->getRef(0);// reset shift cursor //

	for (int i = 0; i < countShifts; i++) // sets up //
		shiftFront.rightShiftCursor(TEXTURE_ATLAS_MAX_WIDTH);

	// iterate through each img startin with startPos until last img into the data strcuture //
	for (; it != end; it++)
	{
		for (int i = 0; i < it->getSizeAsInt(); i++)
		{
			// transfer data [shift left] //
			gdk_pixbuf_copy_area(pixelBuf,
				shiftFront[AT_COL] * REALMZ_GRID_SIZE, shiftFront[AT_ROW] * REALMZ_GRID_SIZE,
				REALMZ_GRID_SIZE, REALMZ_GRID_SIZE,
				pixelBuf,
				startPos[AT_COL] * REALMZ_GRID_SIZE, startPos[AT_ROW] * REALMZ_GRID_SIZE);
			
			// sets up a new reference//
			startPos.leftShiftCursorNTimes(countShifts, TEXTURE_ATLAS_MAX_WIDTH);
			it->setImgRef(i, startPos);
			startPos.rightShiftCursorNTimes(countShifts, TEXTURE_ATLAS_MAX_WIDTH);		

			// readjust cursors //
			startPos.rightShiftCursor(TEXTURE_ATLAS_MAX_WIDTH);
			shiftFront.rightShiftCursor(TEXTURE_ATLAS_MAX_WIDTH);
		}
		
	}

	// clears the last displaced images //
	for (int i = 0; i < countShifts; i++)
	{
		leftShiftCursor();
		gdk_pixbuf_copy_area(pixelBufClean32x32,
			0, 0,
			REALMZ_GRID_SIZE, REALMZ_GRID_SIZE,
			pixelBuf,
			cursor[AT_COL] * REALMZ_GRID_SIZE, cursor[AT_ROW] * REALMZ_GRID_SIZE);
	}	
}