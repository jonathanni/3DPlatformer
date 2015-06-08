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
			core::vector3df(-900, 1350, -1000), core::vector3df(180, 0, 0), core::vector3df(10, 10, 10));
		floorNode = smgr->addCubeSceneNode(2.0f, NULL, PICKABLE,
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), core::vector3df(10000, 1, 10000));
		flagNode = smgr->addAnimatedMeshSceneNode(loadMesh("flag.b3d"), NULL, NONPICKABLE, 
			core::vector3df(-950, 250, 760), core::vector3df(0, 0, 0), core::vector3df(20, 20, 20));
		levelNode = smgr->addAnimatedMeshSceneNode(loadMesh("level00.b3d"), NULL, PICKABLE,
			core::vector3df(0, 1100, 0), core::vector3df(0, 0, 0), core::vector3df(150, 150, 150));

		fields.push_back(new GravityBox(-1070, 150, 200, 350, 550, 900));
		fields.push_back(new GravityBox(-1070, 150, 80, 230, 400, 550));
		fields.push_back(new GravityBox(-1070, -300, 80, 230, 900, 1050));
		fields.push_back(new GravityBox(-180, 180, 350, 550, -180, 900));
		fields.push_back(new GravityBox(-150, 150, 550, 1250, 700, 900));
		fields.push_back(new GravityBox(-300, -150, 80, 1250, 900, 1200));
		fields.push_back(new GravityBox(-170, 170, 650, 850, 100, 700));
		fields.push_back(new GravityBox(-170, 170, 850, 1220, 100, 250));
		fields.push_back(new GravityBox(-330, -160, 550, 730, -180, 700));
		fields.push_back(new GravityBox(-1050, -750, 950, 1250, -800, 480));
		fields.push_back(new GravityBox(-1050, -750, 1250, 1400, -1050, -430));
		fields.push_back(new MassObject(new float[3]{-800, 1125, 1100}, 10000000));

		//{
		//	float loc[3] = {-750, 850, 1000};
		//	fields.push_back(new MassObject(loc, 1000));
		//}

		((GravityBox*)fields.at(0))->setDownVector(core::vector3d<float>(0, -1, 0));
		((GravityBox*)fields.at(1))->setDownVector(core::vector3d<float>(0, 0, 1));
		((GravityBox*)fields.at(2))->setDownVector(core::vector3d<float>(0, 0, -1));
		((GravityBox*)fields.at(3))->setDownVector(core::vector3d<float>(0, 1, 0));
		((GravityBox*)fields.at(4))->setDownVector(core::vector3d<float>(0, 0, 1));
		((GravityBox*)fields.at(5))->setDownVector(core::vector3d<float>(1, 0, 0));
		((GravityBox*)fields.at(6))->setDownVector(core::vector3d<float>(0, -1, 0));
		((GravityBox*)fields.at(7))->setDownVector(core::vector3d<float>(0, 0, -1));
		((GravityBox*)fields.at(8))->setDownVector(core::vector3d<float>(1, 0, 0));
		((GravityBox*)fields.at(9))->setDownVector(core::vector3d<float>(0, 1, 0));
		((GravityBox*)fields.at(10))->setDownVector(core::vector3d<float>(0, 1, 0));

		rot[0] = 0;
		rot[1] = 0;
		rot[2] = 0;

		velocity.set(0, 0, 0);

		cursor = device->getCursorControl();

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

		//	camera = smgr->addCameraSceneNodeFPS(0, 100, 0.4f, CAMERA, keyMap, 6, true, 3.0f);
			camera = smgr->addCameraSceneNode(NULL, core::vector3df(-870, 400, 760), core::vector3df(0, 0, 1));
			camera->setPosition(core::vector3df(-870, 400, 760));
			camera->setTarget(core::vector3df(0, 0, 0));
			camera->setFarValue(5000);
		//	camera->bindTargetAndRotation(true);
			collider = smgr->createCollisionResponseAnimator(metaSelector, camera,
				core::vector3df(20, 60, 20), core::vector3df(0, 0, 0), core::vector3df(0, 0, 0));

			metaSelector->drop();
			floorNodeSelector->drop();
			levelNodeSelector->drop();
			camera->bindTargetAndRotation(true);
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

	void Platformer::rotateCamera(int x, int y, core::vector3df downvector){
		float new_x = x - (800 / 2);
		float new_y = y - (600 / 2);
		if (downvector.Y != 0){
			rot[0] += new_y*PLATFORMER_ROTATE_SPEED*-downvector.Y;
			rot[1] += new_x*PLATFORMER_ROTATE_SPEED*-downvector.Y;
			if (rot[0] > 89.9){
				rot[0] = 89.9;
			}if (rot[0] < -89.9){
				rot[0] = -89.9;
			}
			rot[2] = 0;
		}if (downvector.Z != 0){
			rot[1] += -new_y*PLATFORMER_ROTATE_SPEED*downvector.Z;
			rot[0] += new_x*PLATFORMER_ROTATE_SPEED*downvector.Z;
			if (rot[1] > 89.9){
				rot[1] = 89.9;
			}if (rot[1] < -89.9){
				rot[1] = -89.9;
			}
		}
		cursor->setPosition(400, 300);
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
		//	drawBoundingBoxes();

			guienv->drawAll();

			driver->endScene();
		}
	}
	core::vector3df toEuler(double x, double y, double z, double angle) {
		double s = sin(angle);
		double c = cos(angle);
		double t = 1 - c;
		core::vector3df rot;
		//  if axis is not already normalised then uncomment this
		// double magnitude = Math.sqrt(x*x + y*y + z*z);
		// if (magnitude==0) throw error;
		// x /= magnitude;
		// y /= magnitude;
		// z /= magnitude;
		if ((x*y*t + z*s) > 0.998) { // north pole singularity detected
			rot.X = 2 * atan2(x*sin(angle / 2), cos(angle / 2));
			rot.Y = 3.14 / 2;
			rot.Z = 0;
			return rot;
		}
		if ((x*y*t + z*s) < -0.998) { // south pole singularity detected
			rot.X = -2 * atan2(x*sin(angle / 2), cos(angle / 2));
			rot.Y = -3.14 / 2;
			rot.Z = 0;
			return rot;
		}
		rot.X = atan2(y * s - x * z * t, 1 - (y*y + z*z) * t);
		rot.Y = asin(x * y * t + z * s);
		rot.Z = atan2(x * s - y * z * t, 1 - (x*x + z*z) * t);
		return rot;
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
			}

			if (totalDownVector == core::vector3d<float>(0, 0, 0))
			{
				velocity.set(0, 0, 0);
				totalDownVector = core::vector3d<float>(0, -1, 0);
			}

			if (!(velocity.equals(core::vector3df(0, 0, 0))))
			velocity += totalDownVector;

			//camera->updateAbsolutePosition();
			rotateCamera(cursor->getPosition().X, cursor->getPosition().Y, totalDownVector);
			

			core::vector3df upvec = camera->getUpVector();
			upvec = upvec.normalize();
			normalizedDownVector = totalDownVector.normalize();
			core::vector3df vec(0, 1, 0);
			core::vector3df axis = vec.crossProduct(-normalizedDownVector);;
			float num = vec.dotProduct(-normalizedDownVector);
			
			num = acos(num);
			
			core::vector3df rotation = toEuler(axis.X, axis.Y, axis.Z, num)*(180 / 3.14);
			log << rot[0] << endl;
		//	camera->setRotation(camera->getRotation() + rotation);
			camera->setRotation(core::vector3df(rot[0], rot[1], rot[2]));
			
			if (normalizedDownVector != temp)
			{
				temp = normalizedDownVector;
				camera->setUpVector(-normalizedDownVector);
				
			}

			core::vector3df up = getSurfaceTri(camera->getPosition(), totalDownVector.normalize())
				.getNormal().normalize() * PLATFORMER_JUMP_FORCE;
				
			irr:core::matrix4 mat = camera->getRelativeTransformation();
			
			core::vector3d<float> lookat = core::vector3df(mat[8], mat[9], mat[10]);
			core::vector3d<float> leftvector = core::vector3df(mat[0], mat[1], mat[2]);

			core::vector3df dir = -normalizedDownVector.crossProduct(lookat.crossProduct(-normalizedDownVector));
			core::vector3df leftdir = normalizedDownVector.crossProduct(leftvector.crossProduct(normalizedDownVector));

			lookat.normalize();
			leftvector.normalize();
			if (spaceBarEvent.IsKeyDown(irr::KEY_SPACE))
			{
				collider->setGravity(core::vector3df(0, 0, 0));
				velocity = (up)+(1 / PLATFORMER_TIME_CONSTANT)*totalDownVector;
			}
			else
				collider->setGravity(totalDownVector * 1000);

			

			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_W)){
				camera->setPosition(camera->getAbsolutePosition() + dir*PLATFORMER_SPEED);
				camera->updateAbsolutePosition();
				camera->setRotation(core::vector3df(rot[0], rot[1], rot[2]));
			}
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_S)){
				camera->setPosition(camera->getAbsolutePosition() + -dir*PLATFORMER_SPEED);
				camera->updateAbsolutePosition();
				camera->setRotation(core::vector3df(rot[0], rot[1], rot[2]));
			}
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_A)){
				camera->setPosition(camera->getAbsolutePosition() + -leftdir * PLATFORMER_SPEED);
				camera->updateAbsolutePosition();
				camera->setRotation(core::vector3df(rot[0], rot[1], rot[2]));
			}
			if (spaceBarEvent.IsKeyDown(irr::KEY_KEY_D)){
				camera->setPosition(camera->getAbsolutePosition() + leftdir * PLATFORMER_SPEED);
				camera->updateAbsolutePosition();
				camera->setRotation(core::vector3df(rot[0], rot[1], rot[2]));
			}

			camera->setPosition(camera->getPosition() + velocity);
			camera->updateAbsolutePosition();
			camera->setRotation(core::vector3df(rot[0], rot[1], rot[2]));
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