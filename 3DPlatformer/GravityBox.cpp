#include "GravityBox.h"
#include <irrlicht.h>

namespace Platformer
{
	using namespace irr;
	core::vector3d<float> GravityBox::calcDownVector(core::vector3d<float> cameraPos)
	{
		if (cameraPos[0] > minX && cameraPos[0] < maxX && cameraPos[1] > minY && cameraPos[1] < maxY && cameraPos[2] > minZ && cameraPos[2] < maxZ){		
			return downVector;
		}
		else{
			core::vector3d<float> zero;
			zero.set(0, 0, 0);
			return zero;
		}
	}

	void GravityBox::setDownVector(core::vector3d<float> vec)
	{
		downVector.set(vec);
	}

	GravityBox::GravityBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ){
		this->minX = minX;
		this->maxX = maxX;
		this->minY = minY;
		this->maxY = maxY;
		this->minZ = minZ;
		this->maxZ = maxZ;
	}

} 
