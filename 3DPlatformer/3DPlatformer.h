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

		IrrlichtDevice * device;

		bool success = true;
	private:
		IVideoDriver* driver;
		ISceneManager* smgr;
		IGUIEnvironment* guienv;

		IAnimatedMesh* testMesh;
	};

	bool check(Platformer *);
}

#endif