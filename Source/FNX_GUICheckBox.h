#pragma once
#include "fnx_guibaseelement.h"
#include "FNX_Sprite.h"
#include "FNX_Font.h"

class FNX_GUICheckBox : public FNX_GUIBaseElement
{
protected:
	FNX_Sprite *Check,*Box;
	FNX_Sprite* BackGround;
	FNX_SpriteManager* SpriteManager;
	FNX_Font* Font;
	std::string Text;
	bool Checked;
	int FWidth,FHeight;///font
	DWORD Color;
	bool temp;
public:
	FNX_GUICheckBox(FNX_Logger* pLogger);
	~FNX_GUICheckBox(){if(SpriteManager){SpriteManager->~FNX_SpriteManager();SpriteManager=NULL;};if(Font){Font->~FNX_Font();Font=NULL;};};

	bool IsChecked(){return Checked;};
	bool Update_This(FNX_Input* Input,bool ByMouse=true);
	void Render_This();
	void SetText(std::string& sText){Text=sText;};
	void SetTextColor(DWORD sColor){Color=sColor;};
	void Invalidate(){SpriteManager->Invalidate();Font->Invalidate();};
	void Release(){delete this;};

	void Restore(){SpriteManager->Restore();Font->Restore();};
	void LoadFromFile(std::string FileName);
	void SetChecked(bool v){Checked=v;};
};