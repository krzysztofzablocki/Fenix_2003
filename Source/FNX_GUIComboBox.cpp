#include "FNX_GUIComboBox.h"
#include "FNX_FileOperations.h"



FNX_GUIComboBox::FNX_GUIComboBox(FNX_Logger *pLogger)
{
	Logger=pLogger;
	SpriteManager=new FNX_SpriteManager(Logger,100);
	
	UpBelt=SpriteManager->Add("",false);
	Button=SpriteManager->Add("",false);
	ChoosenLine=-1;
	
	ListBox= new FNX_GUIListBox(Logger);
	Add_Element(static_cast<FNX_GUIBaseElement*>(ListBox));

	//ListBox->SetOnChangeFunction(ListChange);

	

	Scale=FNX_Vector(100,100);

	DropDownCount=8;
	DropedDown=false;
	temp=false;

	Font=ListBox->GetFont();
	ListBox->SetMultiSelect(false);
};


void FNX_GUIComboBox::LoadFromFile(std::string FileName)
{
	ListBox->LoadFromFile(FileName);
	Font=ListBox->GetFont();
	
	SpriteManager->~FNX_SpriteManager();
	SpriteManager=new FNX_SpriteManager(Logger,100);
	
	FNX_FileOperator FileOp(FileName);

	std::string Texture=FileOp.ReadStringFromFile("Texture");

	UpBelt=SpriteManager->Add(Texture);
	Button=SpriteManager->Add(Texture);

	UpBelt->SetUniqueMapping(FileOp.ReadVectorFromFile("UpBeltMSLU"),FileOp.ReadVectorFromFile("UpBeltMERD"));

	Button->SetUniqueMapping(FileOp.ReadVectorFromFile("ButtonMSLU"),FileOp.ReadVectorFromFile("ButtonMERD"));

};


bool FNX_GUIComboBox::Update_This(FNX_Input *Input, bool ByMouse)
{
	if(ByMouse)
	{
	
		
		if(!Input->LastFramePressed)
			{
				if(Button->TouchesPoint(Input->MousePos))
				{
					temp=true;
					return true;
				};
			};


	if(Input->LastFramePressed)
	{
		if(!BUTTONDOWN(Input->MouseState,0))
		{
			if(Button->TouchesPoint(Input->MousePos))
			{
				temp=false;
				
				DropedDown=!DropedDown;
				EventOccured(FNX_GUIEvent_OnClick);

				return true;
			};

			temp=false;
		};

	};

return false;
};




	
	return false;
};




void FNX_GUIComboBox::Render_This()
{
	if(!GetFocus())DropedDown=false;

	ListBox->SetScale(FNX_Vector(Scale.x,(float)Font->GetHeight()*DropDownCount+10));
	ListBox->SetPosition(FNX_Vector(Position.x,Position.y-ListBox->GetScale().y/2.0f-Font->GetHeight()*1.5f/2.0f));

	UpBelt->SetScale(Scale.x-20,Font->GetHeight()*1.5f);
	UpBelt->SetPosition(Position.x-10,Position.y);

	Button->SetScale(20,UpBelt->GetScale().y);
	Button->SetPosition(Position.x+UpBelt->GetScale().x/2.0f,Position.y);
	Button->SetColor(D3DCOLOR_XRGB(255,255,255));


	if(!DropedDown)ListBox->SetActive(false);else ListBox->SetActive(true);
	if(DropedDown)if(ListBox->WasClicked())DropedDown=!DropedDown;else ListBox->WasClicked();


	SpriteManager->Render();

	FNX_Vector Poz=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),UpBelt->GetPosition().x-UpBelt->GetScale().x/2+5,UpBelt->GetPosition().y-UpBelt->GetScale().y/2);
	FNX_Vector Poz2=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),UpBelt->GetPosition().x+UpBelt->GetScale().x/2,UpBelt->GetPosition().y+UpBelt->GetScale().y/2-5);
	
	

	std::vector<int> s=ListBox->GetSelectedItemNr();
	if(s.size()>0)ChoosenLine=s[0];

	if(ChoosenLine>=0)Font->RenderText(ListBox->GetLine(ChoosenLine).c_str(),-1,&FNX_SetRect((LONG)Poz.x,(LONG)Poz2.x,(LONG)Poz.y,(LONG)Poz2.y),DT_SINGLELINE,D3DCOLOR_XRGB(0,0,0));
};

