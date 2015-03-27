#ifndef _3DPLATFORMER_H
#define _3DPLATFORMER_H

namespace Platformer{

	// Irrlicht namespaces
	using namespace irr;
	using namespace core;
	using namespace scene;
	using namespace video;
	using namespace io;
	using namespace gui;

	class Platformer{
	public:
		Platformer(void);
		~Platformer(void);

		void init(void);
		void run(void);
		void update(void);

		void start(void);
		void stop(void);

		IrrlichtDevice * device;

		bool success = true;
	private:
		IVideoDriver * driver;
		ISceneManager * smgr;
		IGUIEnvironment * guienv;

		ICameraSceneNode * camera;

		bool isFloor = false, isUpdate = false;

		std::thread * updateThread;
	};

	bool check(Platformer *);
}

#endif