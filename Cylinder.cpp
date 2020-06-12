#include "Cylinder.h"
#include "Thing.h"
#include "MapResources.h"

extern data::MapResources* gResources;

const data::Thing& scene::Cylinder::operator[](int index)
{
	return items[index];
}

const data::Thing& scene::Cylinder::getTopItem()
{
	return items[items.size() - 1];
}

void scene::Cylinder::addItem(data::Thing thing)
{
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