#ifndef _3DPLATFORMER_H
#define _3DPLATFORMER_H

#include "IGravityField.h"
#include "MyEventListener.h"
#define PLATFORMER_TIME_CONSTANT 20
#define PLATFORMER_RAY_LIMIT 10000
#define PLATFORMER_JUMP_FORCE 20
#define PLATFORMER_SPEED 30

namespace Platformer{

	using namespace irr;
	using namespace std;

	class Platformer{
	public:
		Platformer(void);
		~Platformer(void);

		void init(void);
		void run(void);
		void update(void);

		void start(void);
		void stop(void);

		irr::IrrlichtDevice *device;

					
		

		bool success = true;
	private:
		scene::IAnimatedMesh *loadMesh(char *);
		core::triangle3df getSurfaceTri(core::vector3df, core::vector3df);

		// For Debug //
		void drawBoundingBoxes(void);

		static ofstream log;
		//           //

		video::IVideoDriver *driver;
		scene::ISceneManager *smgr;
		gui::IGUIEnvironment *guienv;

		scene::ICameraSceneNode *camera;
		scene::ISceneNodeAnimatorCollisionResponse * collider;
		scene::IAnimatedMeshSceneNode *treeNode, *portalNode;
		scene::ISceneNode *floorNode;
		scene::ISceneNode *gravityBox;
		scene::ILightSceneNode *sun;
		scene::ISceneNode *sunController, *cameraController;
		MyEventReceiver spaceBarEvent;
		bool isFloor = false, isUpdate = false;
		core::vector3df cameraPlane;
		
		std::vector<scene::ISceneNode*> sceneNodes;
		std::thread *updateThread;
		
		vector<IGravityField*> fields;
		core::vector3df velocity;
	};

	bool check(Platformer *);
}

#endif