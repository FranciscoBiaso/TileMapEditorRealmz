#include "Cylinder.h"
#include "Thing.h"
#include "MapResources.h"

extern data::MapResources* gResources;

scene::Cylinder::Cylinder(){}

const data::Thing& scene::Cylinder::operator[](int index)
{
	return items[index];
}

const data::Thing& scene::Cylinder::getTopItem()
{
	return items[items.size() - 1];
}


void scene::Cylinder::setCoords(int x, int y, int z)
{
	coords.setXYZ(x, y, z);
}

math::Vec3<int> scene::Cylinder::getCoords()
{
	return coords;
}

void scene::Cylinder::addItem(data::Thing thing)
{
	thing.setCylinder(this);
	items.push_back(thing);
	std::sort(items.begin(), items.end(), [](data::Thing& a, data::Thing& b) -> bool
		{			
			return gResources->getLayerAsInt(a.getType()) > gResources->getLayerAsInt(b.getType());			
		});
}

void scene::Cylinder::removeItem(std::string name)
{
	std::vector<data::Thing>::iterator it = std::find_if(items.begin(), items.end(), [&, name](data::Thing& thing) {
		if (thing.getName() == name)
			return true;
		return false;
		});
	
	if (it != items.end())
	{
		items.erase(it);
	}
}


void scene::Cylinder::draw(cairo_t* cr)
{
	for (auto it = items.begin(); it != items.end(); it++)
	{
		(*it).draw(cr);
	}
}