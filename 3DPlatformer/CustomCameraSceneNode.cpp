#ifdef _WIN32
#include "stdafx.h"
#endif

#include <irrlicht.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "CustomCameraSceneNode.h"
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

namespace Platformer 
{

}