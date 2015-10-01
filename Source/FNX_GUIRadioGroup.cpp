#include "FNX_GUIRadioGroup.h"



void FNX_GUIRadioGroup::Render_This()
{
	bool t=false;
	for(int i=0;i<(int)CheckBoxes.size();++i)
	{
		
	
	if(last!=-1)
		if(CheckBoxes[i]->IsChecked())
		if(i!=last)CheckBoxes[i]->SetChecked(false);

	if(CheckBoxes[i]->IsChecked())t=true;
	
	CheckBoxes[i]->Render();
	};
	
	
if(!t && last>=0 && last<(int)CheckBoxes.size())CheckBoxes[last]->SetChecked(true);
	
};

bool FNX_GUIRadioGroup::Update_This(FNX_Input* Input,bool ByMouse)
{
if(ByMouse)
{
	for(int i=0;i<(int)CheckBoxes.size();++i)
	{
		bool c=CheckBoxes[i]->IsChecked();
		bool d=CheckBoxes[i]->Update(Input);
		if(d)
		{
			last=-1;
			if(c!=CheckBoxes[i]->IsChecked()) last=i;
			return true;
		};
	};

return false;
};


return false;
};



