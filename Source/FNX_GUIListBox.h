#pragma once
#include "FNX_GUIBaseElement.h"
#include "FNX_Sprite.h"
#include "FNX_GUIScrollBar.h"
#include "FNX_Logger.h"
#include <vector>
#include <string>
#include "FNX_Font.h"
#include "FNX_Timer.h"

class FNX_GUIListBox : public FNX_GUIBaseElement
{
protected:
	FNX_Sprite* BackGround,*Choosen;
	FNX_SpriteManager* SpriteManager;
	FNX_SpriteManager* SpriteManager2;
	FNX_Timer Timer;

	FNX_GUIScrollBar* ScrollBar;
	FNX_Logger* Logger;
	FNX_Font* Font;

	std::vector<std::string>Lines;
	std::vector<int>ChoosenList;

	bool MultiSelect;
	int ChoosenLine;
	bool HaveChanged;

	float TT;
	DWORD Color;
public:
	FNX_GUIListBox(FNX_Logger* pLogger);
	~FNX_GUIListBox(){SpriteManager->~FNX_SpriteManager();SpriteManager2->~FNX_SpriteManager();Font->~FNX_Font();ScrollBar->~FNX_GUIScrollBar();};

	void Render_This();
	bool Update_This(FNX_Input* Input,bool ByMouse=true);
	void Invalidate();
	void Restore();
	void LoadFromFile(std::string FileName);
	void SetMultiSelect(bool val){MultiSelect=val;};
	
	void AddLine(std::string Line){Lines.push_back(Line);};

	std::vector<int> GetSelectedItemNr(){return ChoosenList;};
	std::vector<std::string> GetSelectedItemStrings();

	std::string GetLine(int index){if(index<0 || index>=(int)Lines.size())return NULL; return Lines[index];};
	FNX_Font* GetFont(){return Font;};

	bool WasClicked(){bool b=HaveChanged; HaveChanged=false;return b;};


	void Release(){delete this;};


	bool GetFocus(){return (FNX_GUIBaseElement::GetFocus() || ScrollBar->GetFocus());};
};