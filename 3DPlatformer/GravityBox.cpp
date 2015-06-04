#include "GravityBox.h"
#include <irrlicht.h>
#include <iostream>
namespace Platformer
{
	using namespace irr;
	core::vector3d<float> GravityBox::calcDownVector(core::vector3d<float> cameraPos)
	{
		if (cameraPos.X > bounds.MinEdge.X && cameraPos.X < bounds.MaxEdge.X && 
			cameraPos.Y > bounds.MinEdge.Y && cameraPos.Y < bounds.MaxEdge.Y && 
			cameraPos.Z > bounds.MinEdge.Z && cameraPos.Z < bounds.MaxEdge.Z)
			return downVector;
		else
			return core::vector3d<float>(NAN, NAN, NAN);
	}

	core::vector3d<float> GravityBox::calcDownVector1(core::vector3d<float> cameraPos)
	{
		return downVector;
	}

	core::aabbox3d<float> GravityBox::getBounds()
	{
		return bounds;
	}

	void GravityBox::setDownVector(core::vector3d<float> vec)
	{
		downVector.X = vec.X;
		downVector.Y = vec.Y;
		downVector.Z = vec.Z;
	}

	GravityBox::GravityBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
	{
		bounds = core::aabbox3d<float>(minX, minY, minZ, maxX, maxY, maxZ);
	}

} 
