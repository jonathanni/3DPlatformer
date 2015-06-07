#include "MassObject.h"
#include <irrlicht.h>
#include <fstream>
namespace Platformer
{
	using namespace irr;

	core::vector3d<float> MassObject::calcDownVector(core::vector3d<float> cameraPos)
	{ 
		
		//core::vector3d<float> difference = cameraPos - position;
		core::vector3d<float> downVector;

		downVector = position - cameraPos;
		
		if (downVector.getLengthSQ() <= 250000){
			return core::vector3d<float>(NAN, NAN, NAN);
		}
		
		
		return downVector;
	}
	core::vector3d<float> MassObject::calcDownVector1(core::vector3d<float> cameraPos)
	{
		
		//core::vector3d<float> difference = cameraPos - position;
		core::vector3d<float> downVector;
		
		downVector = position - cameraPos;
		//downVector = cameraPos - position;
		if (downVector.getLengthSQ() > 250000){
			return core::vector3df(0, 0, 0);
		}
		float force = mass / downVector.getLengthSQ();
		
		downVector.normalize();
		downVector *= force;

		return downVector;
	}

	core::aabbox3d<float> MassObject::getBounds()
	{
		const float radius = sqrt(mass / PLATFORMER_GRAVITY_DEBUG_LIMIT);
		return core::aabbox3d<float>(position.X - radius, position.Y - radius, position.Z - radius,
									 position.X + radius, position.Y + radius, position.Z + radius);
	}

	MassObject::MassObject(float pos[3], float m)
	{
		position.X = pos[0];
		position.Y = pos[1];
		position.Z = pos[2];
		mass = m;
		
	}
}
