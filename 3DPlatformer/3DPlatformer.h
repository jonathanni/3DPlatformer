#ifndef _3DPLATFORMER_H
#define _3DPLATFORMER_H

#include "IGravityField.h"

#define PLATFORMER_TIME_CONSTANT 20
#define PLATFORMER_RAY_LIMIT 10000

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

		// For Debug
		void drawBoundingBoxes(void);

		video::IVideoDriver *driver;
		scene::ISceneManager *smgr;
		gui::IGUIEnvironment *guienv;

		scene::ICameraSceneNode *camera;
		scene::ISceneNodeAnimatorCollisionResponse * collider;
		scene::IAnimatedMeshSceneNode *treeNode, *portalNode;
		scene::ISceneNode *floorNode;
		scene::ISceneNode *gravityBox;
		scene::ILightSceneNode *sun;
		scene::ISceneNode *sunController;

		bool isFloor = false, isUpdate = false;
		
		std::vector<scene::ISceneNode*> sceneNodes;
		std::thread *updateThread;
		
		vector<IGravityField*> fields;
		core::vector3df velocity;
	};

	bool check(Platformer *);
}

#endif