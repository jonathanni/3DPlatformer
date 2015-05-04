#include "GravityBox.h"
#include <irrlicht.h>
#include <iostream>
namespace Platformer
{
	using namespace irr;
	core::vector3d<float> GravityBox::calcDownVector(core::vector3d<float> cameraPos)
	{
		std::cout << "hi!" << std::endl;
		if (cameraPos.X > minX && cameraPos.X < maxX && cameraPos.Y > minY && cameraPos.Y < maxY && cameraPos.Z > minZ && cameraPos.Z < maxZ){		
			
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
		downVector.X = vec.X;
		downVector.Y = vec.Y;
		downVector.Z = vec.Z;
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
