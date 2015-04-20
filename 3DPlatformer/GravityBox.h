#ifndef _GRAVITYBOX_H
#define _GRAVITYBOX_H
#include "IGravityField.h"
namespace Platformer
{
	using namespace irr;
	class GravityBox : public IGravityField
	{
		private:
			core::vector3d<float> downVector;
		public:
			void setDownVector(core::vector3d<float>);
			core::vector3d<float> calcDownVector();
	};
}

#endif