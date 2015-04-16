#ifndef _GRAVITYBOX_H
#define _GRAVITYBOX_H
#include "IGravityField.h"
namespace Platformer
{
	using namespace irr;
	class GravityBox : public IGravityField
	{
	public:
		core::vector3d<float> calcDownVector();
	};
}

#endif