#include "FNX_Font.h"
#include <d3dx9.h>

RECT FNX_SetRect(LONG Left,LONG Right,LONG Bottom,LONG Top)
{
	RECT rect;
	rect.left=Left;	rect.right=Right;
	rect.top=Top;	rect.bottom=Bottom;
 return rect;
};

FNX_Font::FNX_Font(FNX_Logger* Log,UINT height,UINT width,BOOL IsItalic,LPCSTR FontName)
{
	Height=height;
	Width=width;
	Logger=Log;	
	HRESULT result=D3DXCreateFontA(Logger->GetD3DDevice(),height,width,BOLD_FONTTYPE,2,IsItalic,DEFAULT_CHARSET,OUT_TT_ONLY_PRECIS,0,DEFAULT_PITCH,FontName,&Font);
	Logger->Log("Creating Font",result);
	inited=true;
};

void FNX_Font::RenderText(LPCTSTR pString, INT Count, LPRECT pRect, DWORD Format, D3DCOLOR Color)
{
	if(inited)Font->DrawText(NULL,pString,Count,pRect,Format,Color);
};