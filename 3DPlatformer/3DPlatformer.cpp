// 3DPlatformer.cpp : Defines the entry point for the application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <chrono>
#include <thread>

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
		E_DRIVER_TYPE driverType;

#ifdef _WIN32
		driverType = EDT_DIRECT3D9;
#else
		driverType = EDT_OPENGL;
#endif

		device =
			createDevice(driverType, dimension2d<u32>(800, 600), 16,
			false, true, false, NULL);

		if (!device)
			success = false;
	}

	Platformer::~Platformer()
	{
		if (success){
			stop();

			delete updateThread;
		}
	}

	void Platformer::init(void)
	{
		device->setWindowCaption(L"3D Platformer");

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();

		smgr->setAmbientLight(SColorf(0x00c0c0c0));

		IMeshSceneNode * floorNode = smgr->addCubeSceneNode(2.0f, NULL, 0, vector3df(0, -1, 0), vector3df(0, 0, 0), vector3df(1000, 1, 1000));

		floorNode->setMaterialFlag(video::EMF_LIGHTING, true);

		floorNode->getMaterial(0).DiffuseColor.set(0xff000000);
		floorNode->getMaterial(0).AmbientColor.set(0xff404040);
		floorNode->getMaterial(0).Shininess = 0;

		ITriangleSelector * floorNodeSelector = smgr->createOctreeTriangleSelector(floorNode->getMesh(), floorNode, 12);

		floorNode->setTriangleSelector(floorNodeSelector);

		IMetaTriangleSelector * metaSelector = smgr->createMetaTriangleSelector();

		metaSelector->addTriangleSelector(floorNodeSelector);

		{
			SKeyMap keyMap[6];

			keyMap[0].Action = EKA_MOVE_FORWARD;
			keyMap[0].KeyCode = KEY_KEY_W;
			keyMap[1].Action = EKA_MOVE_BACKWARD;
			keyMap[1].KeyCode = KEY_KEY_S;
			keyMap[2].Action = EKA_STRAFE_LEFT;
			keyMap[2].KeyCode = KEY_KEY_A;
			keyMap[3].Action = EKA_STRAFE_RIGHT;
			keyMap[3].KeyCode = KEY_KEY_D;

			keyMap[4].Action = EKA_JUMP_UP;
			keyMap[4].KeyCode = KEY_SPACE;
			keyMap[5].Action = EKA_CROUCH;
			keyMap[5].KeyCode = KEY_LSHIFT;

			camera = smgr->addCameraSceneNodeFPS(0, 100, 0.4f, -1, keyMap, 6, true, 3.0f);

			camera->setPosition(vector3df(5, 1.6f, 5));
			camera->setTarget(vector3df(0, 0, 0));
			camera->setFarValue(5000);

			scene::ISceneNodeAnimatorCollisionResponse * collider =
				smgr->createCollisionResponseAnimator(metaSelector, camera, vector3df(1.0f, 1.6f, 1.0f), vector3df(0, -9.8f, 0), vector3df(0, 1.6f, 0));

			metaSelector->drop();
			floorNodeSelector->drop();

			camera->addAnimator(collider);
			collider->drop();
		}
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

	void Platformer::update()
	{
		while (isUpdate)
		{
			isFloor = camera->getPosition().Y <= 0;

			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	}

	// start update thread -> start graphics -> stop update thread -> stop graphics

	void Platformer::start()
	{
		isUpdate = true;

		// Create separate thread first
		updateThread = new std::thread(&Platformer::update, this);

		// Runs in separate loop
		run();
	}

	void Platformer::stop()
	{
		isUpdate = false;

		updateThread->join();

		device->closeDevice();
		device->run();
		device->drop();
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

	pMain->start();

	delete pMain;
	return 0;
}