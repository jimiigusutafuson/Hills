#include "Input.h"

Input *Input::_this = NULL;
Input::Input(HINSTANCE hInstance, HWND *hWnd )
{
	directInput			= 0;
	keyboardInput		= 0;
	mouseInput			= 0;
	DirectInput8Create(hInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void**)&directInput,
								NULL);
	initKeyboard(hWnd);
	initMouse(hWnd);
}
Input* Input::createInstance(HINSTANCE hInstance, HWND *hWnd)
{
	if(!_this)
		_this = new Input(hInstance, hWnd);
	return _this;
}
Input* Input::getInstance()
{
	if(_this)
		return _this;
	else return NULL;
}
HRESULT Input::initKeyboard(HWND* hWnd)
{
	HRESULT result;
	if(FAILED( result = directInput->CreateDevice(	GUID_SysKeyboard,
									&keyboardInput,
									NULL )))
		return result;

	if(FAILED(result = keyboardInput->SetDataFormat( &c_dfDIKeyboard )))
		return result;

	if(FAILED(result = keyboardInput->SetCooperativeLevel( *hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )))
		return result;

	result = keyboardInput->Acquire();
	return result;
}
HRESULT Input::initMouse(HWND* hWnd)
{
	HRESULT result;
	if(FAILED( result = directInput->CreateDevice(	GUID_SysMouse,
								&mouseInput,
								NULL )))
		return result;

	if(FAILED(result = mouseInput->SetDataFormat( &c_dfDIMouse )))
		return result;

	if(FAILED(result = mouseInput->SetCooperativeLevel( *hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE )))
		return result;

	result = mouseInput->Acquire();
	return result;
}
void Input::update()
{
	for( int i = 0; i < 256; i++)
		prevKeyState[i] = keyState[i];
	prevMouseState = mouseState;

	HRESULT result;
	result = keyboardInput->GetDeviceState( 256, (LPVOID)&keyState );

	if(FAILED(result))
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			keyboardInput->Acquire();

	result = mouseInput->GetDeviceState( sizeof(DIMOUSESTATE), (LPVOID)&mouseState );
	
	if(FAILED(result))
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			mouseInput->Acquire();
}
Input::~Input()
{
}
