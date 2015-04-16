#ifndef _IGRAVITYFIELD_H
#define _IGRAVITYFIELD_H

#include <irrlicht.h>
namespace Platformer

{
	using namespace irr;
	
	class IGravityField{
	public:
		
		virtual core::vector3d<float> calcDownVector(core::vector3d<float>) = 0;
	};

}


#endif 