#ifndef _IGRAVITYFIELD_H
#define _IGRAVITYFIELD_H

#include <irrlicht.h>
#include <math.h>
#define GRAVITY 6.67*pow(10.0, -5.0)
namespace Platformer

{
	using namespace irr;
	
	class IGravityField{
	public:
		
		virtual core::vector3d<float> calcDownVector(core::vector3d<float>) = 0;
	};

}


#endif 