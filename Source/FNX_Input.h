#pragma once
#define DIRECTINPUT_VERSION  0x0800

#include <DInput.h>
#include "FNX_Logger.h"


#define KEYDOWN(name, key) (name[key] & 0x80) 
#define BUTTONDOWN(name, key) (name.rgbButtons[key] & 0x80)

class FNX_Input
{
protected:
	LPDIRECTINPUT8 DirectInput;
	LPDIRECTINPUTDEVICE8 KeyboardDevice;
	LPDIRECTINPUTDEVICE8 MouseDevice;
	

	void (*KeyboardFunc)(char* Buffer);
	void (*MouseFunc)(DIMOUSESTATE*);

	bool MouseCreated;
	bool KeyboardCreated;
	FNX_Logger* Logger;
	float SensitivyScale;
	bool ACMouse;//acquiring mouse
	

	FNX_Vector MousePositionPressed;///mouse position when button was pressed

	char PressedChar;
public:
	bool LastFramePressed;///if it was last frame pressed
	FNX_Vector MouseMovement;///mouse movement in last update
	DIMOUSESTATE MouseState,oldMouseState;
	char buffer[256];
	char oldbuffer[256];
	FNX_Vector MousePos;
	
	~FNX_Input();

	FNX_Input(FNX_Logger* pLogger,bool Keyboard,bool Mouse,bool MouseExclusive=false,bool KeyboardExclusive=false);
	void SetKeyboardFunction(void (*pKeyboardFunction)(char* Buffor)){KeyboardFunc=pKeyboardFunction;};
	void SetMouseFunction(void (*pMouseFunction)(DIMOUSESTATE* s)){MouseFunc=pMouseFunction;};
	void GuardMessagesLoop(UINT,WPARAM,LPARAM);///use it on beggining of message loop
	void SetSensitivyScale(float val){SensitivyScale=val;};
	void Update();
	
	void UnacquireMouse(){if(MouseCreated)MouseDevice->Unacquire();ACMouse=false;};//unacquire and stop acquiring
	void AcquireMouse(){if(MouseCreated)MouseDevice->Acquire(); ACMouse=true;};
	bool IsAcquired(){return ACMouse;};



	char GetChar(){char s=PressedChar;PressedChar=0;return s;};//return 0 if no char
	void TakeOverChars(WPARAM wParam){PressedChar=static_cast<char>(wParam);};



	void Release(){delete this;};

};