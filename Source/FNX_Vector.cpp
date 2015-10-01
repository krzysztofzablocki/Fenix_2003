#include "FNX_Vector.h"

FNX_Vector FNX_TranslatePosOffScreenLHToLH(HWND hWnd,float x,float y)
{
	RECT Window;
	GetClientRect(hWnd,&Window);

	return FNX_Vector(x-(Window.right/2),-(y-(Window.bottom/2)));

};

FNX_Vector FNX_TranslatePosLHToOffScreenLH(HWND hWnd,float x,float y)
{
	RECT Window;
	GetClientRect(hWnd,&Window);

	return FNX_Vector(x+(Window.right/2),-y+(Window.bottom/2));
};