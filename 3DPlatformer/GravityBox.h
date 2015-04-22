#ifndef _GRAVITYBOX_H
#define _GRAVITYBOX_H
#include "IGravityField.h"

#define MASS 5
namespace Platformer
{
	using namespace irr;
	class GravityBox : public IGravityField
	{
		private:
			core::vector3d<float> downVector;
			float minX, maxX, minY, maxY, minZ, maxZ;
		public:
			void setDownVector(core::vector3d<float>);
			core::vector3d<float> calcDownVector(core::vector3d<float>);
			GravityBox(float, float, float, float, float, float);
			
	};
}

#endif