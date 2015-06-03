#include "MassObject.h"
#include <irrlicht.h>

namespace Platformer
{
	using namespace irr;

	core::vector3d<float> MassObject::calcDownVector(core::vector3d<float> cameraPos)
	{ 
		if ((cameraPos.X < position.X + 100 && cameraPos.X > position.X - 100) &&
			(cameraPos.Y < position.Y + 100 && cameraPos.Y > position.Y - 100) &&
			(cameraPos.Z < position.Z + 100 && cameraPos.Z > position.Z - 100)) {
			return core::vector3d<float>(NAN, NAN, NAN);
		}
		
		core::vector3d<float> downVector;
		
		downVector = cameraPos - position;
		
		float force = mass / downVector.getLengthSQ();
		
		downVector.normalize();
		downVector *= -force; 

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
