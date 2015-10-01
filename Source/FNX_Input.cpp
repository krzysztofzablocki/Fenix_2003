#include "FNX_Input.h"

FNX_Input::~FNX_Input()
{
if(DirectInput)
{
	if(MouseDevice)
	{
		MouseDevice->Unacquire();
		MouseDevice->Release();
		MouseDevice=NULL;
	}

	if(KeyboardDevice)
	{
		KeyboardDevice->Unacquire();
		KeyboardDevice->Release();
		KeyboardDevice=NULL;
	};

	DirectInput->Release();
	DirectInput=NULL;
};
};


FNX_Input::FNX_Input(FNX_Logger* pLogger,bool Keyboard,bool Mouse,bool MouseExclusive,bool KeyboardExclusive)
{	
POINT posMouse;
GetCursorPos(&posMouse);
RECT wind;
GetWindowRect(pLogger->GethWnd(),&wind);
MousePos=FNX_Vector((float)(posMouse.x-400),(float)(300-posMouse.y+8));

KeyboardFunc=NULL;MouseFunc=NULL;

KeyboardCreated=MouseCreated=LastFramePressed=false;
Logger=pLogger;
SensitivyScale=1.0f;

DWORD MFlags=DISCL_FOREGROUND|DISCL_NONEXCLUSIVE;
if(MouseExclusive)MFlags=DISCL_FOREGROUND|DISCL_EXCLUSIVE;

DWORD KFlags=DISCL_FOREGROUND|DISCL_NONEXCLUSIVE;
if(KeyboardExclusive)KFlags=DISCL_FOREGROUND|DISCL_EXCLUSIVE;


	if SUCCEEDED(DirectInput8Create(Logger->GethInstance(),DIRECTINPUT_VERSION, 
		IID_IDirectInput8,(void**)&DirectInput,NULL))
	{
		if(Keyboard)
		if SUCCEEDED(DirectInput->CreateDevice(GUID_SysKeyboard,&KeyboardDevice,NULL))
		{
			if FAILED(KeyboardDevice->SetDataFormat(&c_dfDIKeyboard))return;
			if FAILED(KeyboardDevice->SetCooperativeLevel(Logger->GethWnd(), KFlags))return;
			KeyboardDevice->Acquire();
			KeyboardCreated=true;
			Logger->Log("Creating Keyboard",S_OK);
		};

		if(Mouse)
		if SUCCEEDED(DirectInput->CreateDevice(GUID_SysMouse,&MouseDevice,NULL))
		{
			if FAILED(MouseDevice->SetDataFormat(&c_dfDIMouse))return;
			if FAILED(MouseDevice->SetCooperativeLevel(Logger->GethWnd(),MFlags))return;
			
			MouseDevice->Acquire();
			MouseCreated=true;
			ACMouse=true;
		
			Logger->Log("Creating Mouse",S_OK);
		};

	};


	ZeroMemory(&MouseState,sizeof(MouseState));
	ZeroMemory(&oldMouseState,sizeof(MouseState));
};

void FNX_Input::Update()
{
	if(KeyboardCreated)
	{

		memcpy(oldbuffer,buffer,sizeof(buffer));
	
/*
		for(int i=0;i<255;++i)
		{
			if(buffer[i] & 0x80)
			{
			//	PressedChar=static_cast<char>(i);
				break;
			};
		};

*/
		//Zero buffer
		ZeroMemory(buffer,sizeof(buffer));

		if FAILED(KeyboardDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer))
		{
			KeyboardDevice->Acquire();
			return;
		};

		if(KeyboardFunc!=NULL)(*KeyboardFunc)(buffer);
	};

	if(MouseCreated)
	{
		
		LastFramePressed=BUTTONDOWN(MouseState,0);//check if it was pressed
		oldMouseState=MouseState;
		if FAILED(MouseDevice->GetDeviceState(sizeof(MouseState),(LPVOID)&MouseState))
		{
		if(ACMouse)	MouseDevice->Acquire();
			return;
		};

		MousePos.x+=MouseState.lX*SensitivyScale;
		MousePos.y-=MouseState.lY*SensitivyScale;
		
		MouseMovement=FNX_Vector(MouseState.lX*SensitivyScale,MouseState.lY*SensitivyScale);
		
		if(!LastFramePressed && BUTTONDOWN(MouseState,0))
		{
			MousePositionPressed=MousePos;
		};


	if (MouseFunc!=NULL)(*MouseFunc)(&MouseState);

	
	};

};

void FNX_Input::GuardMessagesLoop(UINT Message,WPARAM wParam,LPARAM lParam)
{
switch(Message)
{
	case WM_ACTIVATE:
    if (WA_INACTIVE == wParam)
	{ 
		MouseDevice->Unacquire();
		ACMouse=false;
	}
    else
	{
		MouseDevice->Acquire();
		ACMouse=true;
	};
	break;
	case WM_ENTERMENULOOP:
		MouseDevice->Unacquire();
		ACMouse=false;
		break;
	case WM_EXITSIZEMOVE:
		MouseDevice->Acquire();
ACMouse=true;	
		break;

}
};