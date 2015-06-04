// 3DPlatformer.cpp : Defines the entry point for the application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>

#include <assert.h>

#include <irrlicht.h>

#include "3DPlatformer.h"
#include "GravityBox.h"
#include "MassObject.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

namespace Platformer
{
	enum NodeType
	{
		CAMERA = 0,
		LIGHT = 1,
		PICKABLE = 2,
		NONPICKABLE = 3
	};

	ofstream Platformer::log;

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
			false, true, false, &spaceBarEvent);
		
		log = ofstream("debug.log");
		
		if (!device || !log.is_open())
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

		sun = smgr->addLightSceneNode();
		sun->setID(LIGHT);
		sun->getLightData().Type = video::ELT_DIRECTIONAL;

		sunController = smgr->addEmptySceneNode();
		sunController->setID(LIGHT);

		sun->setPosition(core::vector3df(0, 0, 1));
		sun->setParent(sunController);

		sunController->setRotation(core::vector3df(-90, -90, 0));

		treeNode = smgr->addAnimatedMeshSceneNode(loadMesh("tree00.b3d"), NULL, PICKABLE,
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(10, 10, 10));
		portalNode = smgr->addAnimatedMeshSceneNode(loadMesh("portal.b3d"), NULL, PICKABLE,
			core::vector3df(100, 10, 100), core::vector3df(0, 0, 0), core::vector3df(10, 10, 10));
		floorNode = smgr->addCubeSceneNode(2.0f, NULL, PICKABLE,
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(10000, 1, 10000));
		flagNode = smgr->addAnimatedMeshSceneNode(loadMesh("flag.b3d"), NULL, NONPICKABLE, 
			core::vector3df(-950, 250, 760), core::vector3df(0, 0, 0), core::vector3df(20, 20, 20));
		levelNode = smgr->addAnimatedMeshSceneNode(loadMesh("level00.b3d"), NULL, PICKABLE,
			core::vector3df(0, 1100, 0), core::vector3df(0, 0, 0), core::vector3df(150, 150, 150));

		//fields.push_back(new GravityBox(-10000, 10000, -10000, 10000, -10000, 10000));
		fields.push_back(new GravityBox(-1070, 150, 200, 450, 550, 900));
		fields.push_back(new GravityBox(-1070, 150, 100, 250, 400, 550));
		//fields.push_back(new MassObject(new float[3]{0, 0, 0}, 100000));

		((GravityBox*)fields.at(0))->setDownVector(core::vector3d<float>(0, -1, 0));
		((GravityBox*)fields.at(1))->setDownVector(core::vector3d<float>(0, 0, 1));

		velocity.set(0, 0, 0);

		sceneNodes.push_back(sun);
		sceneNodes.push_back(sunController);
		sceneNodes.push_back(floorNode);
		sceneNodes.push_back(treeNode);
		sceneNodes.push_back(portalNode);
		sceneNodes.push_back(flagNode);
		sceneNodes.push_back(levelNode);

		treeNode->setMaterialFlag(video::EMF_LIGHTING, true);
		treeNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		treeNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

		portalNode->setMaterialFlag(video::EMF_LIGHTING, true);

		portalNode->setJointMode(scene::EJUOR_CONTROL);
		portalNode->setDebugDataVisible(scene::EDS_SKELETON);

		{
			scene::ISceneNodeAnimator *rot =
				smgr->createRotationAnimator(core::vector3df(0, 1.0f, 0)), 
				*trans = smgr->createFlyStraightAnimator(portalNode->getPosition(),
				                                         portalNode->getPosition() + core::vector3df(0, 50, 0),
														 2000, true, true);
			portalNode->addAnimator(rot);
			portalNode->addAnimator(trans);
			
			rot->drop();
			trans->drop();
		}

		floorNode->setMaterialFlag(video::EMF_LIGHTING, true);
		floorNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

		floorNode->getMaterial(0).DiffuseColor.set(0xff000000);
		floorNode->getMaterial(0).AmbientColor.set(0xff404040);
		floorNode->getMaterial(0).Shininess = 0;

		flagNode->setMaterialFlag(video::EMF_LIGHTING, true);
		flagNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		flagNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

		flagNode->setAnimationSpeed(24);
		flagNode->setFrameLoop(59, 119);

		levelNode->setMaterialFlag(video::EMF_LIGHTING, true);
		levelNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		levelNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		
		scene::ITriangleSelector *floorNodeSelector = smgr->createOctreeTriangleSelector(
			((scene::IMeshSceneNode *)floorNode)->getMesh(), floorNode, 12),
								 *levelNodeSelector = smgr->createOctreeTriangleSelector(
			levelNode->getMesh(), levelNode, 64);

		floorNode->setTriangleSelector(floorNodeSelector);
		levelNode->setTriangleSelector(levelNodeSelector);

		scene::IMetaTriangleSelector *metaSelector = smgr->createMetaTriangleSelector();
		cameraPlane = core::vector3df(1, 0, 0);
		metaSelector->addTriangleSelector(floorNodeSelector);
		metaSelector->addTriangleSelector(levelNodeSelector);

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

			//camera = smgr->addCameraSceneNode(0, core::vector3df(1000, 1000, 1000), core::vector3df(0, -100, 0), -1, true);

			camera = smgr->addCameraSceneNodeFPS(0, 100, 0.4f, CAMERA, keyMap, 6, true, 3.0f);

			camera->setPosition(core::vector3df(-870, 400, 760));
			camera->setTarget(core::vector3df(0, 0, 0));
			camera->setFarValue(5000);

			collider = smgr->createCollisionResponseAnimator(metaSelector, camera,
				core::vector3df(20, 60, 20), core::vector3df(0, 0, 0), core::vector3df(0, 0, 0));

			metaSelector->drop();
			floorNodeSelector->drop();
			levelNodeSelector->drop();

			camera->addAnimator(collider);
			collider->drop();
			
			//cameraController = smgr->addEmptySceneNode();
			//cameraController->setPosition(camera->getPosition());
			//cameraController->setRotation(camera->getRotation());
			//camera->setParent(cameraController);
			//cameraController->setRotation(cameraPlane);
		}
	}

	scene::IAnimatedMesh* Platformer::loadMesh(char * path)
	{
		scene::IAnimatedMesh *mesh = smgr->getMesh(path);

		if (!mesh)
		{
			device->drop();
			exit(1);
		}

		return mesh;
	}

	core::triangle3df Platformer::getSurfaceTri(core::vector3df pos, core::vector3df dir)
	{
		core::line3df ray = core::line3df(pos, pos + dir * PLATFORMER_RAY_LIMIT);

		core::vector3df out;
		core::triangle3df outTri;

		smgr->getSceneCollisionManager()->
			getSceneNodeAndCollisionPointFromRay(ray, out, outTri, PICKABLE);

		return outTri;
	}

	void Platformer::drawBoundingBoxes()
	{
		for (scene::ISceneNode *i : sceneNodes)
			driver->draw3DBox(i->getBoundingBox(), video::SColor(0xffff0000));
		
		driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

		for (IGravityField *i : fields) {
			core::aabbox3d<float> b = i->getBounds();
			log << b.MinEdge.X << " " << b.MinEdge.Y << " " << b.MinEdge.Z << 
				" -> " << b.MaxEdge.X << " " << b.MaxEdge.Y << " " << b.MaxEdge.Z << endl;
			driver->draw3DBox(i->getBounds(), video::SColor(0xff00ff00));
		}
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

			bool isFloor = true;
			core::vector3d<float> totalDownVector;
			totalDownVector.set(0, 0, 0);

			this_thread::sleep_for(chrono::milliseconds(PLATFORMER_TIME_CONSTANT));
			core::vector3d<float> add, add1;

			for (IGravityField *i : fields)
			{
				add = i->calcDownVector(camera->getPosition());
				add1 = i->calcDownVector1(camera->getPosition());

				totalDownVector = totalDownVector + add1;
				// is NaN
				if (!add.equals(add))
				{
					//totalDownVector.set(0, 0, 0);
					velocity.set(0, 0, 0);
					isFloor = true;
					break;
				}

				
			}

			// Included this check so that if the ground is being hit, the downVector still is being
			// calculated (so its not 0), but doesnt affect the velocity of the object
			if (!isFloor){
				velocity.set(0, 0, 0);
			}
			else{
			velocity += totalDownVector;
			}


				core::vector3df upvec = camera->getUpVector();
				normalizedDownVector = totalDownVector.normalize();
				if (normalizedDownVector != temp){
					
					temp = normalizedDownVector;
					camera->setUpVector(-normalizedDownVector);

				}
			core::vector3df up = getSurfaceTri(camera->getPosition(), totalDownVector.normalize())
				.getNormal().normalize() * PLATFORMER_JUMP_FORCE;
				
					//	log << "***************************************************" << isFloor << endl;

		irr:core::matrix4 mat = camera->getRelativeTransformation();
			
			core::vector3d<float> lookat = core::vector3df(mat[8], mat[9], mat[10]);
			core::vector3d<float> leftvector = core::vector3df(mat[0], mat[1], mat[2]);
			core::vector3df dir = -normalizedDownVector.crossProduct(lookat.crossProduct(-normalizedDownVector));
			lookat.normalize();
			leftvector.normalize();
			if (spaceBarEvent.IsKeyDown(irr::KEY_SPACE))
				velocity = up + (1 / PLATFORMER_TIME_CONSTANT) * totalDownVector;
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_W))
				camera->setPosition(camera->getPosition() + dir*PLATFORMER_SPEED);
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_S))
				camera->setPosition(camera->getPosition() + -dir*PLATFORMER_SPEED);
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_A))
				camera->setPosition(camera->getPosition() + -leftvector * PLATFORMER_SPEED);
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_D))
				camera->setPosition(camera->getPosition() + leftvector * PLATFORMER_SPEED);
			//device->getEventReceiver()->OnEvent()
			camera->setPosition(camera->getPosition() + velocity);
			
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

		log.close();

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