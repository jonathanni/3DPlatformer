#ifndef _MYEVENTLISTENER_H
#define _MYEVENTLISTENER_H
#include <irrlicht.h>
using namespace irr;
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


#endif _MYEVENTLISTENER_H