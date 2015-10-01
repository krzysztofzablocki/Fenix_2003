#include "FNX_GUICheckBox.h"
#include "FNX_FileOperations.h"
#include "FNX_Font.h"

FNX_GUICheckBox::FNX_GUICheckBox(FNX_Logger *pLogger)
{
	Logger=pLogger;

	SpriteManager=new FNX_SpriteManager(Logger,100);

	Check=SpriteManager->Add("",false);
	Box=SpriteManager->Add("",false);
	BackGround=SpriteManager->Add("",false);

	Checked=false;

	Font=new FNX_Font(Logger,12,12,false,"Lucida Console");
	Scale=FNX_Vector(50,20);
	Color=D3DCOLOR_XRGB(255,0,0);
	Text="CheckBox";
	Position=FNX_Vector(0,0);
	temp=false;
}

void FNX_GUICheckBox::Render_This()
{

if(Checked)
{
	Box->SetVisible(false);
	Check->SetVisible(true);
}else
{
	Box->SetVisible(true);
	Check->SetVisible(false);
};

	Box->SetScale(FNX_Vector(20,20));
	BackGround->SetPosition(Position-FNX_Vector(Box->GetScale().x/2,0));BackGround->SetScale(Scale.x-Box->GetScale().x,Scale.y);
	Box->SetPosition(Position-FNX_Vector(BackGround->GetScale().x/2+Box->GetScale().x,0));
	Check->SetPosition(Box->GetPosition());Check->SetScale(Box->GetScale());

	SpriteManager->Render();

	FNX_Vector Poz=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),BackGround->GetPosition().x-BackGround->GetScale().x/2,BackGround->GetPosition().y-BackGround->GetScale().y/4);
	FNX_Vector Poz2=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),BackGround->GetPosition().x+BackGround->GetScale().x/2,BackGround->GetPosition().y+BackGround->GetScale().y/4);
	
		Font->RenderText(Text.c_str(),-1,&FNX_SetRect((LONG)Poz.x
												  ,(LONG)Poz2.x,
												(LONG)Poz.y, 
												  (LONG)Poz2.y),
												  DT_SINGLELINE,Color);
};





bool FNX_GUICheckBox::Update_This(FNX_Input* Input,bool ByMouse)
{

if(ByMouse)
{
	if(!Input->LastFramePressed)
	{
		if(Box->TouchesPoint(Input->MousePos) || BackGround->TouchesPoint(Input->MousePos))
		{
			temp=true;
			return true;
		};
	};

	if(Input->LastFramePressed)
	{
		if(!BUTTONDOWN(Input->MouseState,0))
		{
			if(Box->TouchesPoint(Input->MousePos)|| BackGround->TouchesPoint(Input->MousePos))
			{
				temp=false;
				Checked=!Checked;
				EventOccured(FNX_GUIEvent_OnClick);
				EventOccured(FNX_GUIEvent_OnChange);
				return true;
			};

			temp=false;
		};

	};



	return false;

};


return false;
};


void FNX_GUICheckBox::LoadFromFile(std::string FileName)
{
	SpriteManager->~FNX_SpriteManager();
	SpriteManager=new FNX_SpriteManager(Logger,100);
	Font->~FNX_Font();
	Font=NULL;

	FNX_FileOperator FileOp(FileName);

	std::string Texture=FileOp.ReadStringFromFile("Texture");

	Box=SpriteManager->Add(Texture);Box->SetColor(D3DCOLOR_XRGB(255,255,255));
	BackGround=SpriteManager->Add(Texture);BackGround->SetColor(D3DCOLOR_XRGB(255,255,255));
	Check=SpriteManager->Add(Texture);Check->SetColor(D3DCOLOR_XRGB(255,255,255));

	Box->SetUniqueMapping(FileOp.ReadVectorFromFile("BoxMSLU"),FileOp.ReadVectorFromFile("BoxMERD"));
	
	BackGround->SetUniqueMapping(FileOp.ReadVectorFromFile("BackGroundMSLU"),FileOp.ReadVectorFromFile("BackGroundMERD"));
	
	Check->SetUniqueMapping(FileOp.ReadVectorFromFile("CheckMSLU"),FileOp.ReadVectorFromFile("CheckMERD"));

	std::string FontName=FileOp.ReadStringFromFile("FontName");
	int h=FileOp.ReadIntFromFile("FontHeight");
	int w=FileOp.ReadIntFromFile("FontWidth");

	Font=new FNX_Font(Logger,h,w,false,FontName.c_str());

	Color=FileOp.ReadColorARGBFromFile("FontColor");

};