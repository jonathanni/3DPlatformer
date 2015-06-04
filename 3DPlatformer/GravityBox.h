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
			core::aabbox3d<float> bounds;
		public:
			void setDownVector(core::vector3d<float>);

			core::vector3d<float> calcDownVector(core::vector3d<float>);
			core::vector3d<float> calcDownVector1(core::vector3d<float>);

			core::aabbox3d<float> getBounds(void);

			GravityBox(float, float, float, float, float, float);
			
	};
}

#endif