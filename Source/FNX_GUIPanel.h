#pragma once
#include "FNX_GUIBaseElement.h"
#include "FNX_Input.h"


class FNX_GUIPanel : public FNX_GUIBaseElement
{
protected:
		///no render for now
	
public:
	FNX_GUIPanel(){};
	//~FNX_GUIPanel();
	void Invalidate();
	void Restore();
	void Release(){delete this;};
	bool Update_This(FNX_Input* Input,bool ByMouse=true){return false;};
	void Render_This(){};
	void LoadFromFile(std::string FileName);
	void SetPosition(FNX_Vector& s);
};