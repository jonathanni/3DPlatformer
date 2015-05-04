#ifndef _CUSTOMCAMERA_H
#define _CUSTOMCAMERA_H
#include <vector>
#include <irrlicht.h>

#include "IGravityField.h"
namespace Platformer{
	class CustomCamera{
	private:
		std::vector<IGravityField> nodes;

	public:

		CustomCamera(void);
	
		~CustomCamera(void);

	};
}

#endif