#ifndef _3DPLATFORMER_H
#define _3DPLATFORMER_H

#include "IGravityField.h"
#include "MyEventListener.h"

#define PLATFORMER_TIME_CONSTANT 20
#define PLATFORMER_RAY_LIMIT 10000
#define PLATFORMER_JUMP_FORCE 90
#define PLATFORMER_SPEED 30
#define PLATFORMER_ROTATE_SPEED .3f
#define PLATFORMER_TREE_COUNT 2

namespace Platformer
{
	
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
					
		static ofstream log;

		bool success = true;
	private:
		scene::IAnimatedMesh *loadMesh(char *);
		core::triangle3df getSurfaceTri(core::vector3df, core::vector3df);

		// For Debug //
		void drawBoundingBoxes(void);
		void rotateCamera(int, int, core::vector3df);
		
		//           //

		video::IVideoDriver *driver;
		scene::ISceneManager *smgr;
		gui::IGUIEnvironment *guienv;

		scene::ICameraSceneNode *camera;
		scene::ISceneNodeAnimatorCollisionResponse * collider;
		scene::IMeshSceneNode *treeNode[PLATFORMER_TREE_COUNT], *portalNode, *flagNode, *levelNode;
		scene::ISceneNode *floorNode;
		gui::ICursorControl *cursor;
		scene::ISceneNode *gravityBox;
		scene::ILightSceneNode *sun;
		scene::ISceneNode *sunController, *cameraController;
		MyEventReceiver spaceBarEvent;
		bool isFloor = false, isUpdate = false;
		core::vector3df cameraPlane;
		core::vector3df normalizedDownVector;
		core::vector3df temp;
		std::vector<scene::ISceneNode*> sceneNodes;
		std::thread *updateThread;
		float rot[3];
		vector<IGravityField*> fields;
		core::vector3df velocity;
	};

	bool check(Platformer *);
}

#endif