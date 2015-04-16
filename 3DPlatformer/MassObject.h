#ifndef _MASSOBJECT_H
#define _MASSOBJECT_H
#include "IGravityField.h"
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
		MassObject(float[3], float);
	};
}

#endif