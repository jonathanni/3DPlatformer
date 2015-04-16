#include "MassObject.h"
#include <irrlicht.h>

namespace Platformer
{
	using namespace irr;
	core::vector3d<float> MassObject::calcDownVector(core::vector3d<float> cameraPos){
		core::vector3d<float> downVector;
		downVector = cameraPos - downVector;
		downVector.normalize();
		float force = downVector.getLengthSQ();
		core::vector3d<float> ret;
		return ret;

	}

	MassObject::MassObject(float pos[3]){
		
		position.X = pos[0];
		position.Y = pos[1];
		position.Z = pos[2];

	}
}
