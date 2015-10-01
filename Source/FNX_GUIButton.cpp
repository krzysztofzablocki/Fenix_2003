#include "FNX_GUIButton.h"
#include "FNX_FileOperations.h"

FNX_GUIButton::FNX_GUIButton(FNX_Logger* pLogger)
{
	Logger=pLogger;
	SpriteManager=new FNX_SpriteManager(Logger,100);
	Graphic=SpriteManager->Add("",false);
	OnPressed=NULL;
	Scale=FNX_Vector(30,30);
	Position.Zero();
	temp=false;
	
};

void FNX_GUIButton::Render_This()
{
	FNX_Vector Pos=Position;
	if(temp)Pos+=FNX_Vector(1,1);

	Graphic->SetPosition(Pos);
	Graphic->SetScale(Scale);
	
	SpriteManager->Render();
};

bool FNX_GUIButton::Update_This(FNX_Input* Input,bool ByMouse)
{

if(ByMouse)
{
	if(!Input->LastFramePressed)
	{
		if(Graphic->TouchesPoint(Input->MousePos))
		{
			temp=true;
			return true;
		};
	};


	if(Input->LastFramePressed)
	{
		if(!BUTTONDOWN(Input->MouseState,0))
		{
			if(Graphic->TouchesPoint(Input->MousePos))
			{
				temp=false;
				EventOccured(FNX_GUIEvent_OnClick);
				return true;
			};

			temp=false;
		};

	};

return false;
};


//ByKeyboard
return false;
};



void FNX_GUIButton::LoadFromFile(std::string FileName)
{
	SpriteManager->~FNX_SpriteManager();
	SpriteManager=new FNX_SpriteManager(Logger,100);

	FNX_FileOperator FileOp(FileName);

	std::string Texture=FileOp.ReadStringFromFile("Texture");

	Graphic=SpriteManager->Add(Texture);Graphic->SetColor(D3DCOLOR_XRGB(255,255,255));
	
	Graphic->SetUniqueMapping(FileOp.ReadVectorFromFile("ButtonMSLU"),FileOp.ReadVectorFromFile("ButtonMERD"));

};