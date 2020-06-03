#include "ImgObj.h"

data::ImgObj::ImgObj(const std::vector<math::Vec2<int>>& references)
{
	for (int i = 0; i < references.size(); i++)
		imgs[i] = references[i];
}