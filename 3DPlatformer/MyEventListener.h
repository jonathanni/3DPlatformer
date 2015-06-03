#ifndef _MYEVENTLISTENER_H
#define _MYEVENTLISTENER_H
#include <irrlicht.h>
using namespace irr;
namespace Platformer
{
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
						case KEY_KEY_W:
							break;
						case KEY_KEY_A:
							break;
						case KEY_KEY_S:
							break;
						case KEY_KEY_D:
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
}

#endif _MYEVENTLISTENER_H