#include "GravityBox.h"
#include <irrlicht.h>

namespace Platformer
{
	using namespace irr;
	core::vector3d<float> GravityBox::calcDownVector()
	{
		return downVector;
	}

	void GravityBox::setDownVector(core::vector3d<float> vec)
	{
		downVector.set(vec);
	}

} 
