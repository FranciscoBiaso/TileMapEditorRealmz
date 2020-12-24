#include "Cylinder.h"
#include "Thing.h"
#include "MapResources.h"

extern data::MapResources* gResources;

scene::Cylinder::Cylinder()
{
	_hasLight = true;
}

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

data::Thing scene::Cylinder::addItem(data::Thing thing)
{
	thing.setCylinder(this);

	// work only with one layer //
	//cleans();
	//items.push_back(thing);
	//return thing;


	// \/ work with multiple layers //

	// if item already exists swap them return //
	if (ifItemByLayerAlreadyExistsSwap(thing)) return thing;
	

	// we only push back if we dont have this item //
	items.push_back(thing);
	
	// sorting cylinder //
	std::sort(items.begin(), items.end(), [](data::Thing& a, data::Thing& b) -> bool
		{			
			return gResources->getLayerAsInt(a.getType()) < gResources->getLayerAsInt(b.getType());
		});
	return thing;
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

std::vector<data::Thing>& scene::Cylinder::getItems()
{
	return items;
}


bool scene::Cylinder::hasThingByName(std::string name)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].getStuffBookRefName() == name) return true;
	}
	return false;
}

void scene::Cylinder::setLight(bool value)
{
	this->_hasLight = value;
}

bool scene::Cylinder::hasLight()
{
	return _hasLight;
}

bool scene::Cylinder::isEmpty()
{
	return this->items.empty();
}