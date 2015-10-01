#pragma once
#include "FNX_GUIBaseElement.h"
#include "FNX_Sprite.h"
#include "FNX_Font.h"
#include "FNX_Logger.h"
#include "FNX_Timer.h"
#include <string>


class FNX_GUIEditBox : public FNX_GUIBaseElement
{
protected:
	FNX_SpriteManager* SpriteManager,*SpriteManager2;
	FNX_Sprite* BackGround,*Select;
	FNX_Font* Font;
	FNX_Timer Timer;
	std::string Text;

	int CursorChar;
	int FirstVisibleChar;
	float TT,TT2;
	bool WasSelected;
	int First,Last;
	bool MWasSelected;
	bool Click;
	float ClickTT;
	DWORD Color;
public:
	FNX_GUIEditBox(FNX_Logger* pLogger);
	~FNX_GUIEditBox(){SafeRelease(SpriteManager);SafeRelease(SpriteManager2);SafeRelease(Font);};

	bool Update_This(FNX_Input* Input,bool ByMouse=true);
	void Render_This();
	void Invalidate(){SpriteManager->Invalidate();SpriteManager2->Invalidate();Font->Invalidate();};
	void Restore(){SpriteManager->Restore();SpriteManager2->Restore();Font->Restore();};
	void LoadFromFile(std::string FileName);

	

	void Release(){delete this;};
};