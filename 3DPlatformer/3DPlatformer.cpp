// 3DPlatformer.cpp : Defines the entry point for the application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <irrlicht.h>

#include "3DPlatformer.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

namespace Platformer
{
	// Irrlicht namespaces
	using namespace irr;
	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;

	Platformer::Platformer()
	{
		device =
			createDevice(EDT_BURNINGSVIDEO, dimension2d<u32>(800, 600), 16,
			false, false, false, NULL);

		if (!device)
			success = false;
	}

	Platformer::~Platformer()
	{
		if (success){
			device->closeDevice();
			device->run();
			device->drop();
		}
	}

	void Platformer::init(void)
	{
		device->setWindowCaption(L"3D Platformer");

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();

		guienv->addStaticText(L"Lorem ipsum dolor sit amet.",
			rect<s32>(10, 10, 260, 22), true);

		testMesh = smgr->getMesh("testMesh.x");
		if (!testMesh)
		{
			device->drop();
			success = false;
		}
		IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(testMesh);

		if (node)
		{
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMD2Animation(scene::EMAT_STAND);
			node->setMaterialTexture(0, driver->getTexture("dirt.png"));
		}

		smgr->addCameraSceneNode(0, vector3df(2, 2, 2), vector3df(-1, -1, -1));
	}

	void Platformer::run()
	{
		while (device->run())
		{
			driver->beginScene(true, true, SColor(255, 100, 101, 140));

			smgr->drawAll();
			guienv->drawAll();

			driver->endScene();
		}
	}

	bool check(Platformer * p)
	{
		if (!p->success)
		{
			// failure
			delete p;
			return false;
		}

		return true;
	}
}

int main(int argc, char * argv[])
{
	Platformer::Platformer * pMain = new Platformer::Platformer();

	if (!check(pMain))
	{
		std::cerr << "Failed creation." << std::endl;
		return 2;
	}

	pMain->init();

	if (!check(pMain))
	{
		std::cerr << "Failed initialization." << std::endl;
		return 3;
	}

	pMain->run();

	delete pMain;
	return 0;
}