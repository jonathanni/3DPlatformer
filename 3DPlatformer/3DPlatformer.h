#ifndef _3DPLATFORMER_H
#define _3DPLATFORMER_H

#include "IGravityField.h"
#define PLATFORMER_TIME_CONSTANT 20
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
		class MyEventReceiver : public IEventReceiver
		{
		public:
			virtual bool OnEvent(const SEvent& event)
			{
				if (event.EventType == irr::EET_KEY_INPUT_EVENT)
				{
					KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
					{
						if (event.EventType == irr::EET_KEY_INPUT_EVENT&&!event.KeyInput.PressedDown)
							switch (event.KeyInput.Key)
						{
							case KEY_SPACE:
								break;
							case KEY_KEY_2:
								break;
							case KEY_KEY_3:
								break;
						}
						return true;
					}
				}
				return false;
			}

			virtual bool IsKeyDown(EKEY_CODE keyCode) const
			{
				return KeyIsDown[keyCode];
			}

			MyEventReceiver()
			{
				memset(KeyIsDown, false, sizeof(KeyIsDown));
			}

		private:
			bool KeyIsDown[KEY_KEY_CODES_COUNT];
		};
		bool success = true;
	private:
		void drawBoundingBoxes(void);

		video::IVideoDriver *driver;
		scene::ISceneManager *smgr;
		gui::IGUIEnvironment *guienv;

		scene::ICameraSceneNode *camera;

		scene::ISceneNode *treeNode, *floorNode;

		bool isFloor = false, isUpdate = false;
		MyEventReceiver spaceBarEvent;

		std::vector<scene::ISceneNode*> sceneNodes;
		std::thread *updateThread;
		
		vector<IGravityField*> fields;
		core::vector3d<float> velocity;
	};

	bool check(Platformer *);
}

#endif