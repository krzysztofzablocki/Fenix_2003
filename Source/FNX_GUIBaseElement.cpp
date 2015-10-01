#include "FNX_GUIBaseElement.h"


FNX_GUIBaseElement::FNX_GUIBaseElement()
{
Choosen=NULL;
Position.Zero();
Active=true;
OnPressed=OnChange=NULL;
Focus=false;

};

bool FNX_GUIBaseElement::Update(FNX_Input* Input)
{
	if(!Active)return false;

///focus
	for(int i=0;i<(int)Elements.size();++i)Elements[i]->SetFocus(false);
	SetFocus(false);

	if(!None)
	{
		if(Choosen!=NULL)Choosen->SetFocus(true);
		else
			SetFocus(true);
	};
///focus



if(!Input->LastFramePressed && BUTTONDOWN(Input->MouseState,0))
{
None=true;//none is choosen

	for(int i=0;i<(int)Elements.size();++i)
	{
		if(Elements[i]->Update(Input)){None=false;Choosen=Elements[i];return true;}//if interaction with this element then stop updating;
	};
	
	bool r=Update_This(Input);
	if(r){Choosen=NULL;None=false;};//self
	return r;
};



if(Input->LastFramePressed && !None)
{
 if(Choosen!=NULL)
	return Choosen->Update(Input);
 else
	return Update_This(Input);
};


if(!None)
{
 if(Choosen!=NULL)
	return Choosen->Update(Input);
 else
	return Update_This(Input,false);//keyboard update of selected item
};


return false;
};

void FNX_GUIBaseElement::Render()
{
	if(!Active)return;

	Render_This();

	for(int i=0;i<(int)Elements.size();++i)
	{
		Elements[i]->Render();
	}
	
}

FNX_GUIBaseElement::~FNX_GUIBaseElement()
{
	for(int i=0;i<(int)Elements.size();++i)
	{
		SafeRelease(Elements[i]);
	};


};




void FNX_GUIBaseElement::EventOccured(FNX_GUIEvent Event)
{
	switch(Event)
	{

	case FNX_GUIEvent_OnClick:
		if (OnPressed!=NULL)(*OnPressed)();
		break;
	case FNX_GUIEvent_OnChange:
		if (OnChange!=NULL)(*OnChange)();
		break;
	};

};