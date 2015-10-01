#pragma once
#include "fnx_guibaseelement.h"
#include "FNX_GUICheckBox.h"
#include <vector>

class FNX_GUIRadioGroup : public FNX_GUIBaseElement //Radio group is only protecting to check only one checkbox
{
private:
	std::vector<FNX_GUICheckBox*>CheckBoxes;
	int last;
public:
	~FNX_GUIRadioGroup(){for(int i=0;i<(int)CheckBoxes.size();++i)CheckBoxes[i]->~FNX_GUICheckBox();};

	void Render_This();
	bool Update_This(FNX_Input* Input,bool ByMouse=true);
	void LoadFromFile(std::string FileName){};
	void AddRadioButton(FNX_GUICheckBox* s){CheckBoxes.push_back(s);};
	void Invalidate(){for(int i=0;i<(int)CheckBoxes.size();++i)CheckBoxes[i]->Invalidate();};
	void Restore(){for(int i=0;i<(int)CheckBoxes.size();++i)CheckBoxes[i]->Restore();};
	void Release(){delete this;};
};