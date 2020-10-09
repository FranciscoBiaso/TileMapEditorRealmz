#include "Thing.h"
#include "Definitions.h"
#include "MapResources.h"
#include "Cylinder.h"

extern data::MapResources* gResources;
GdkPixbuf* data::Thing::_pixelbuf_unity = nullptr;

data::Thing::Thing() : name(""), type(""), imgObj_ptr(nullptr)
{
	parent = nullptr;
	if (_pixelbuf_unity == nullptr)
	{
		_pixelbuf_unity = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
		gdk_pixbuf_fill(_pixelbuf_unity, 0x01010101); // clean buffer //
	}
}

data::Thing::Thing(std::string name, std::string type) :
	name(name), type(type), imgObj_ptr(nullptr) 
{
	parent = nullptr;
	if (_pixelbuf_unity == nullptr)
	{
		_pixelbuf_unity = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, REALMZ_GRID_SIZE, REALMZ_GRID_SIZE);
		gdk_pixbuf_fill(_pixelbuf_unity, 0x01010101); // clean buffer //
	}

}

data::Thing::Thing(Thing*copy)
{
	parent = copy->parent;
	name = copy->name;
	imgObj_ptr = copy->imgObj_ptr;
	type = copy->type;
	stuffBookRefName = copy->stuffBookRefName;
}

data::Thing::Thing(const Thing& copy) : name(copy.name), imgObj_ptr(copy.imgObj_ptr),
		type(copy.type), parent(copy.parent), stuffBookRefName(copy.stuffBookRefName)
{
}

data::Thing& data::Thing::operator=(const data::Thing& v2)
{
	
	if (this != &v2)
	{
		parent = v2.parent;
		name = v2.name;
		imgObj_ptr = v2.imgObj_ptr;
		type = v2.type;
		stuffBookRefName = v2.stuffBookRefName;
	}	

	return *this;
}

void data::Thing::draw(cairo_t* cr)
{
	if (parent != nullptr && _pixelbuf_unity != nullptr && imgObj_ptr != nullptr)
	{
	/*	std::cout << "imgObj_ptr->getName():"<<imgObj_ptr->getName()<<
			"imgObj_ptr->getRef(0).getX():" << imgObj_ptr->getRef(0).getX() << "," <<
			"imgObj_ptr->getRef(0).getY():" << imgObj_ptr->getRef(0).getY() << std::endl;*/
		//gdk_pixbuf_fill(pixelbuf, 0xffffff00); // clean buffer //
		gdk_pixbuf_copy_area(gResources->getImgPack().getTextureAtlas()->getPixelbuf(),
							imgObj_ptr->getRef(0).getX() * REALMZ_GRID_SIZE, imgObj_ptr->getRef(0).getY() * REALMZ_GRID_SIZE,
							REALMZ_GRID_SIZE, REALMZ_GRID_SIZE, 
							_pixelbuf_unity,
							0, 0);
		gdk_cairo_set_source_pixbuf(cr, _pixelbuf_unity, parent->getCoords().getX() * REALMZ_GRID_SIZE, parent->getCoords().getY() * REALMZ_GRID_SIZE);
		cairo_paint(cr);
	}
}

void data::Thing::setCylinder(scene::Cylinder* cylinder)
{
	parent = cylinder;
}

scene::Cylinder* data::Thing::getCylinder()
{
	return parent;
}