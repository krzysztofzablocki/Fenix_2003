#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include "FNX_Logger.h"
#include <windef.h>

RECT FNX_SetRect(LONG Left,LONG Right,LONG Bottom,LONG Top);

class FNX_Font
{
private:
	LPD3DXFONT Font;
	HRESULT Status;
	FNX_Logger* Logger;
	bool inited;
	UINT Height,Width;
public:

	FNX_Font(FNX_Logger* Log,UINT height,UINT width,BOOL IsItalic,LPCSTR FontName);
	~FNX_Font(){if(inited)Font->Release();};


	void Restore(){if(inited)Font->OnResetDevice();};
	void Invalidate(){if(inited)Font->OnLostDevice();};
	void Release(){delete this;};

	void PreloadText(LPCTSTR pString,INT Count){Font->PreloadText(pString,Count);};
	void RenderText(LPCTSTR pString,INT Count,LPRECT pRect,DWORD Format,D3DCOLOR Color);
	bool GetInited(){return inited;};
	UINT GetHeight(){return Height;};
	UINT GetWidth(){return Width;};

};