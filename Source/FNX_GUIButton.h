#pragma once
#include "FNX_GUIBaseElement.h"
#include "FNX_Sprite.h"


class FNX_GUIButton : public FNX_GUIBaseElement/// like Reset button
{
protected:
	FNX_Sprite* Graphic;
	FNX_SpriteManager* SpriteManager;
	
	bool temp;
public:
	FNX_GUIButton(FNX_Logger* pLogger);
	~FNX_GUIButton(){SpriteManager->~FNX_SpriteManager();};
	void Render_This();
	bool Update_This(FNX_Input* Input,bool ByMouse=true);
	
	void Invalidate(){SpriteManager->Invalidate();};
	void Restore(){SpriteManager->Restore();};
	void LoadFromFile(std::string);

	void Release(){delete this;};
};
