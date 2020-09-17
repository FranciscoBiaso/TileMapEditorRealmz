#include "TextureAtlas.h"
#include "MapResources.h"
#include "AuxUI.h"
GdkPixbuf* data::TextureAtlas::pixelBuf = nullptr;
GdkPixbuf* data::TextureAtlas::pixelBufClean32x32 = nullptr;

extern data::MapResources* gResources;
extern ui::AuxUI* gAuxUI;

data::TextureAtlas::TextureAtlas(int width, int height)
{
	loadTextureAtlasInfoFromJson();
	if (!loadTextureAtlasFromImg())
	{
		pixelBuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, (TEXTURE_ATLAS_MAX_WIDTH + 1) * REALMZ_GRID_SIZE, (TEXTURE_ATLAS_MAX_WIDTH + 1) * REALMZ_GRID_SIZE);
		gdk_pixbuf_fill(pixelBuf, 0x55555544); // clean buffer //
		
		if (pixelBuf == NULL)
			throw def::ReturnMsg::NOT_ENOUGH_MEMORY;

		pixelBufClean32x32 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
		gdk_pixbuf_fill(pixelBufClean32x32, 0x22222200); // clean buffer //

		resetCursor();
		gAuxUI->printMsg("Created a new texture atlas!");
	}
	else
	{
		gAuxUI->printMsg("Load texture atlas from bmp file!");
	}
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
	if (gdk_pixbuf_get_width(srcImg) != 64 || gdk_pixbuf_get_height(srcImg) != 64) // we dont continue if srcImg != (64x64) //
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

void data::TextureAtlas::delImgObj(std::list<data::ImgObj>::iterator it, std::list<data::ImgObj>::iterator end)
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

void data::TextureAtlas::saveAtlasAsImg()
{
	gdk_pixbuf_savev(pixelBuf, "resources/textureAtlas.png", "png", NULL, NULL, NULL);
}

bool data::TextureAtlas::loadTextureAtlasFromImg()
{
	pixelBuf = gdk_pixbuf_new_from_file("resources/textureAtlas.png",NULL);
	return pixelBuf != NULL;
}

void data::TextureAtlas::saveAtlasInfoAsJson()
{
	Json::Value root;
	Json::Value jsonArrayCylinders(Json::arrayValue);
	root["cursor_x"] = cursor.getX();
	root["cursor_y"] = cursor.getY();

	Json::StreamWriterBuilder builder;

	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";
	std::unique_ptr<Json::StreamWriter> writer(
		builder.newStreamWriter());

	std::ofstream ofs("resources//texture_atlas_info.json", std::ofstream::out);// file to read //
	writer->write(root, &ofs);

	ofs.close();
}

bool data::TextureAtlas::loadTextureAtlasInfoFromJson()
{
	std::ifstream ifs("resources//texture_atlas_info.json");// file to read //
	Json::CharReaderBuilder rbuilder;	// reader //
	std::string errs; // to check errors //
	Json::Value jsonObj;
	Json::parseFromStream(rbuilder, ifs, &jsonObj, &errs); // parser //   
	if (!jsonObj.isNull()) // loading img pack //
	{
		cursor.setX(jsonObj["cursor_x"].asInt());
		cursor.setX(jsonObj["cursor_y"].asInt());
	}
}