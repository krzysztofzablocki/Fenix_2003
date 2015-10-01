#pragma once
#include "FNX_GUIBaseElement.h"
#include "FNX_GUIListBox.h"
#include "FNX_Sprite.h"


class FNX_GUIComboBox : public FNX_GUIBaseElement
{
protected:
		FNX_SpriteManager* SpriteManager;
		FNX_Sprite* UpBelt,*Button;
		FNX_GUIListBox* ListBox;
		FNX_Logger* Logger;
		FNX_Font* Font;

		int ChoosenLine;
		bool DropedDown;
		int DropDownCount;
		bool temp;
public:
	FNX_GUIComboBox(FNX_Logger* pLogger);
	~FNX_GUIComboBox(){ListBox->~FNX_GUIListBox();SpriteManager->~FNX_SpriteManager();};

	void Render_This();
	bool Update_This(FNX_Input* Input,bool ByMouse=true);
	void Invalidate(){ListBox->Invalidate();SpriteManager->Invalidate();};
	void Restore(){ListBox->Restore();SpriteManager->Restore();};
	void Release(){delete this;};

	void LoadFromFile(std::string FileName);

	void SetDropDownCount(int val){DropDownCount=val;};
	void AddLine(std::string Line){ListBox->AddLine(Line);};

	std::string GetSelectedItem(){return ListBox->GetLine(ChoosenLine);};
	int GetSelectedNr(){return ChoosenLine;};
	void ListChange(){DropedDown=!DropedDown;};


	bool GetFocus(){return (FNX_GUIBaseElement::GetFocus() || ListBox->GetFocus());};
};