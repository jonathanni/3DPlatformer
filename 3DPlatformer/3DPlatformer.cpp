// 3DPlatformer.cpp : Defines the entry point for the application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <irrlicht.h>

#include "3DPlatformer.h"
#include "GravityBox.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

namespace Platformer
{
	Platformer::Platformer()
	{
		video::E_DRIVER_TYPE driverType;

#ifdef _WIN32
		driverType = video::EDT_DIRECT3D9;
#else
		driverType = video::EDT_OPENGL;
#endif

		device =
			irr::createDevice(driverType, core::dimension2d<u32>(800, 600), 16,
			false, true, false, NULL);

		if (!device)
			success = false;

		sceneNodes = std::vector<scene::ISceneNode*>();
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

		smgr->setAmbientLight(video::SColorf(0x00c0c0c0));
		
		scene::IAnimatedMesh* treeMesh = smgr->getMesh("tree00.b3d");

		if (!treeMesh)
		{
			device->drop();
			exit(1);
		}

		sun = smgr->addLightSceneNode();
		sun->getLightData().Type = video::ELT_DIRECTIONAL;

		sunController = smgr->addEmptySceneNode();
		sun->setPosition(core::vector3df(0, 0, 1));
		sun->setParent(sunController);

		sunController->setRotation(core::vector3df(-90, -90, 0));

		treeNode = smgr->addAnimatedMeshSceneNode(treeMesh, NULL, 1,
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(10, 10, 10));
		floorNode = smgr->addCubeSceneNode(2.0f, NULL, 0,
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(10000, 1, 10000));

		sceneNodes.push_back(sun);
		sceneNodes.push_back(sunController);
		sceneNodes.push_back(floorNode);
		sceneNodes.push_back(treeNode);

		treeNode->setMaterialFlag(video::EMF_LIGHTING, true);
		treeNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		treeNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

		floorNode->setMaterialFlag(video::EMF_LIGHTING, true);
		floorNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

		floorNode->getMaterial(0).DiffuseColor.set(0xff000000);
		floorNode->getMaterial(0).AmbientColor.set(0xff404040);
		floorNode->getMaterial(0).Shininess = 0;

		scene::ITriangleSelector *floorNodeSelector = smgr->createOctreeTriangleSelector(((scene::IMeshSceneNode *)floorNode)->getMesh(), floorNode, 12);

		floorNode->setTriangleSelector(floorNodeSelector);

		scene::IMetaTriangleSelector *metaSelector = smgr->createMetaTriangleSelector();

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

			camera->setPosition(core::vector3df(900, 1000, 900));
			camera->setTarget(core::vector3df(0, 0, 0));
			camera->setFarValue(5000);

			scene::ISceneNodeAnimatorCollisionResponse * collider =
				smgr->createCollisionResponseAnimator(metaSelector, camera,
				core::vector3df(20, 60, 20), core::vector3df(0, -9.8f, 0), core::vector3df(0, 1.6f, 0));

			metaSelector->drop();
			floorNodeSelector->drop();

			camera->addAnimator(collider);
			collider->drop();
		}
	}

	void Platformer::drawBoundingBoxes()
	{
		for (scene::ISceneNode* i : sceneNodes)
			driver->draw3DBox(i->getBoundingBox(), video::SColor(0xffff0000));
	}

	void Platformer::run()
	{
		while (device->run())
		{
			driver->beginScene(true, true, video::SColor(0xff64658c));

			smgr->drawAll();

			// Draw bounding boxes
			drawBoundingBoxes();

			guienv->drawAll();

			driver->endScene();
		}
	}

	void Platformer::update()
	{
		while (isUpdate)
		{
			isFloor = camera->getPosition().Y <= 0;

			this_thread::sleep_for(chrono::milliseconds(PLATFORMER_TIME_CONSTANT));
			//for (IGravityField i : fields){

			//}
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
	Platformer::Platformer *pMain = new Platformer::Platformer();

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