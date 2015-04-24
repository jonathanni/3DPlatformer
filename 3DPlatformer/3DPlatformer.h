#ifndef _3DPLATFORMER_H
#define _3DPLATFORMER_H

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
		void drawBoundingBoxes(void);

		video::IVideoDriver *driver;
		scene::ISceneManager *smgr;
		gui::IGUIEnvironment *guienv;

		scene::ICameraSceneNode *camera;
		scene::ISceneNode *treeNode, *floorNode;
		scene::ILightSceneNode *sun;
		scene::ISceneNode *sunController;

		bool isFloor = false, isUpdate = false;

		std::vector<scene::ISceneNode*> sceneNodes;
		std::thread *updateThread;
	};

	bool check(Platformer *);
}

#endif