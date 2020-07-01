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

bool scene::Cylinder::ifItemByLayerAlreadyExistsSwap(data::Thing & toAdd) // specific map editor function //
{
	// we have at least one item //
	if (!items.empty())
	{	
		// if already has this layer just swaps //
		std::string layer = toAdd.getType();
		std::vector<data::Thing>::iterator it = std::find_if(items.begin(), items.end(), [&, layer](data::Thing& thing) {
			if (thing.getType() == layer)
				return true;
			return false;
			});

		// we already has this item //
		if (it != items.end())
		{
			*it = toAdd; // replace item  //
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void scene::Cylinder::addItem(data::Thing thing)
{
	thing.setCylinder(this);

	// if item already exists swap them return //
	if (ifItemByLayerAlreadyExistsSwap(thing)) return;

	// we only push back if we dont have this item //
	items.push_back(thing);

	// sorting cylinder //
	std::sort(items.begin(), items.end(), [](data::Thing& a, data::Thing& b) -> bool
		{			
			return gResources->getLayerAsInt(a.getType()) < gResources->getLayerAsInt(b.getType());
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

void scene::Cylinder::cleans()
{
	while(!items.empty())
	{
		items.erase(items.begin());
	}
}