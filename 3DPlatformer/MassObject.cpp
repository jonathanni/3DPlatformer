#include "MassObject.h"
#include <irrlicht.h>

namespace Platformer
{
	using namespace irr;
	core::vector3d<float> MassObject::calcDownVector(core::vector3d<float> cameraPos)
	{ 
		//core::vector3d<float> difference = cameraPos - position;
		core::vector3d<float> downVector;

		downVector = cameraPos - position;
		if (downVector.getLengthSQ() <= 250000){
			return core::vector3d<float>(NAN, NAN, NAN);
		}
		
	
		downVector = cameraPos - position;
		
		float force = mass/downVector.getLengthSQ();
		
		downVector.normalize();
		downVector *= -force; 

		return downVector;
	}

	MassObject::MassObject(float pos[3], float m)
	{
		position.X = pos[0];
		position.Y = pos[1];
		position.Z = pos[2];
		mass = m;
	}
}
