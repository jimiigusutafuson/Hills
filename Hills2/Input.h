#pragma once
#include "stdafx.h"
#include <dinput.h>
class Input
{
private:
	static Input			*_this;
	LPDIRECTINPUT8			directInput;

	LPDIRECTINPUTDEVICE8	keyboardInput;
	LPDIRECTINPUTDEVICE8	mouseInput;

	//state
	char					keyState[256], prevKeyState[256];
	DIMOUSESTATE			mouseState, prevMouseState;

public:
	static Input*			getInstance();
	static Input*			createInstance(HINSTANCE hInstance, HWND *hWnd);
	Input(HINSTANCE hInstance, HWND *hWnd );

	HRESULT					initKeyboard(HWND *hWnd);
	HRESULT					initMouse(HWND* hWnd);

	void					update();

	DIMOUSESTATE	*getMouseState()		{ return &mouseState;		}
	DIMOUSESTATE	*getPrevMouseState()	{ return &prevMouseState;	}
	char			*getKeyState()			{ return keyState;			}
	char			*getPrevKeyState()		{ return prevKeyState;		}

	long			getDeltaY()				{ return mouseState.lY;		}
	long			getDeltaX()				{ return mouseState.lX;		}
	long			getDeltaZ()				{ return mouseState.lZ;		}

	~Input();
};

