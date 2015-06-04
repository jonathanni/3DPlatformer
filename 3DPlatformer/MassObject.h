#ifndef _MASSOBJECT_H
#define _MASSOBJECT_H

#include "IGravityField.h"

#define PLATFORMER_GRAVITY_DEBUG_LIMIT 0.0001f

namespace Platformer
{
	using namespace irr;
	class MassObject : public IGravityField
	{
	private:
		core::vector3d<float> position;
		
		float mass;
	public:
		core::vector3d<float> calcDownVector(core::vector3d<float>);
		core::vector3d<float> calcDownVector1(core::vector3d<float>);

		core::aabbox3d<float> getBounds(void);

		MassObject(float[3], float);
	};
}

#endif