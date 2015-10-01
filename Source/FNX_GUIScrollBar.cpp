#include "FNX_GUIScrollBar.h"
#include "FNX_FileOperations.h"
#include <string>



FNX_GUIScrollBar::FNX_GUIScrollBar(FNX_Logger *pLogger,bool IsVertical)
{
	Logger=pLogger;

	SpriteManager=new FNX_SpriteManager(Logger,100);
	
	Strip=SpriteManager->Add("",false);
    DownControl=SpriteManager->Add("",false);
	UpControl=SpriteManager->Add("",false);
	Slider=SpriteManager->Add("");

	DownControl->SetColor(D3DCOLOR_XRGB(0,255,0));
	UpControl->SetColor(D3DCOLOR_XRGB(0,255,0));
	Slider->SetColor(D3DCOLOR_XRGB(255,0,0));
	Min=0;Max=100;
	SPosition=0;
	TypeVertical=IsVertical;
	JumpCount=1;
	tempU=tempD=false;
	TT=0.0f;

}

void FNX_GUIScrollBar::Render_This()
{
	if(SPosition>Max)SPosition=Max;
	if(SPosition<Min)SPosition=Min;

if(TypeVertical)
{
	Strip->SetPosition(Position);Strip->SetScale(Scale-FNX_Vector(0,40));
	DownControl->SetPosition(Position-FNX_Vector(0,Scale.y/2.0f-10));DownControl->SetScale(Scale.x,20);///10 half of control size;	
	UpControl->SetPosition(Position+FNX_Vector(0,Scale.y/2.0f-10));UpControl->SetScale(Scale.x,20);///10 half of control size;	
	
	//Slider constant size
//	Slider->SetScale(Scale.x,((Scale.y-40))/(Max-Min));
	Slider->SetScale(Scale.x,20);

	VScale=(float)(Scale.y-40-Slider->GetScale().y)/(Max-Min);
	Slider->SetPosition(Position+FNX_Vector(0,Scale.y/2.0f-20)-FNX_Vector(0,Slider->GetScale().y/2)-FNX_Vector(0,(SPosition*VScale))+FNX_Vector(0,Min*VScale));


};


if(!TypeVertical)
{
	
	Strip->SetPosition(Position);Strip->SetScale(Scale-FNX_Vector(40,0));
	DownControl->SetPosition(Position-FNX_Vector(Scale.x/2.0f-10,0));DownControl->SetScale(20,Scale.y);///10 half of control size;	
	UpControl->SetPosition(Position+FNX_Vector(Scale.x/2.0f-10,0));UpControl->SetScale(20,Scale.y);///10 half of control size;	

	//Slider->SetScale((10*(Scale.x-40))/(Max-Min),Scale.y);
	Slider->SetScale(20,Scale.y);

	VScale=(float)(Scale.x-40-Slider->GetScale().x)/(Max-Min);
	Slider->SetPosition(Position-FNX_Vector(Scale.x/2.0f-20,0)+FNX_Vector(Slider->GetScale().x/2,0)+FNX_Vector((SPosition*VScale),0)-FNX_Vector(Min*VScale,0));


	FNX_Vector Temp;
	DownControl->SetRotation(D3DXToRadian(-90));Temp=DownControl->GetScale();DownControl->SetScale(Temp.y,Temp.x);
	UpControl->SetRotation(D3DXToRadian(-90));Temp=UpControl->GetScale();UpControl->SetScale(Temp.y,Temp.x);
	Slider->SetRotation(D3DXToRadian(-90));Temp=Slider->GetScale();Slider->SetScale(Temp.y,Temp.x);
};


	SpriteManager->Render();
};


bool FNX_GUIScrollBar::Update_This(FNX_Input* Input,bool ByMouse)
{
if(ByMouse)
{
	

	if(DownControl->TouchesPoint(Input->MousePos))
		if((Input->LastFramePressed && !tempU) || (!Input->LastFramePressed))
	{

if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))	EventOccured(FNX_GUIEvent_OnClick);
				
		if(Timer.GetTime()-TT>PressTime || !Input->LastFramePressed)
		{
		
			if(TypeVertical)SPosition+=JumpCount;
			else
				SPosition-=JumpCount;

				TT=Timer.GetTime();
		};


		if(!Input->LastFramePressed)	
		{
			tempD=true;
			tempU=false;
		
		};

		return true;
	};



	if(UpControl->TouchesPoint(Input->MousePos))
		if((Input->LastFramePressed && !tempD) || (!Input->LastFramePressed))
	{
		
		if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))	EventOccured(FNX_GUIEvent_OnClick);
				

	if(Timer.GetTime()-TT>PressTime || !Input->LastFramePressed)
	{	
		if(TypeVertical)SPosition-=JumpCount;
		else
			SPosition+=JumpCount;

			TT=Timer.GetTime();
	};

		if(!Input->LastFramePressed)
		{
			tempU=true;
			tempD=false;
		};
		return true;
	}




if(!Input->LastFramePressed)
	if( Slider->TouchesPoint(Input->MousePos))
	{
		TempPos=Input->MousePos-Slider->GetPosition();

if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))	EventOccured(FNX_GUIEvent_OnClick);
				


		tempU=false;
		tempD=false;
		return true;
	};

if((Input->LastFramePressed && !tempD && !tempU) || (!Input->LastFramePressed))
if(Input->LastFramePressed && TypeVertical)
	if( Strip->TouchesPoint(FNX_Vector(Slider->GetPosition().x,Input->MousePos.y+Input->MouseMovement.y)) )
	{
		//try to match the point i have chosen to the actual mouse position
		SPosition+=(int)((TempPos.y-(Input->MousePos.y-Slider->GetPosition().y))/VScale);

if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))	EventOccured(FNX_GUIEvent_OnClick);
					
		if(!Input->LastFramePressed)
		{
			tempU=false;
			tempD=false;
		};
		return true;
	};

if((Input->LastFramePressed && !tempD && !tempU) || (!Input->LastFramePressed))
if(Input->LastFramePressed && !TypeVertical)
if( Strip->TouchesPoint(FNX_Vector((Input->MousePos.x+Input->MouseMovement.x),Slider->GetPosition().y)))
{
	//try to match the point i have chosen to the actual mouse position
	SPosition-=(int)((TempPos.x-(Input->MousePos.x-Slider->GetPosition().x))/VScale);
if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))	EventOccured(FNX_GUIEvent_OnClick);
				
	if(!Input->LastFramePressed)
		{
			tempU=false;
			tempD=false;
		};
	return true;
};


if((Input->LastFramePressed && !tempD && !tempU) || (!Input->LastFramePressed))
if(Strip->TouchesPoint(Input->MousePos))
	{
		int c;
		if(TypeVertical)
			c=FNX_Sign(Input->MousePos.y-Slider->GetPosition().y);
		else
			c=FNX_Sign(Input->MousePos.x-Slider->GetPosition().x);

		SPosition-= JumpCount*c;

if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))	EventOccured(FNX_GUIEvent_OnClick);
				

		if(!Input->LastFramePressed)
		{
			tempU=false;
			tempD=false;
		};

		return true;
	};


	return false;



};//ByMouse


return false;

}


void FNX_GUIScrollBar::LoadFromFile(std::string FileName)
{
	SpriteManager->~FNX_SpriteManager();
	SpriteManager=new FNX_SpriteManager(Logger,100);

	FNX_FileOperator FileOp(FileName);
	
	std::string Texture=FileOp.ReadStringFromFile("Texture");

	Strip=SpriteManager->Add(Texture);Strip->SetColor(D3DCOLOR_XRGB(255,255,255));
	DownControl=SpriteManager->Add(Texture);DownControl->SetColor(D3DCOLOR_XRGB(255,255,255));
	UpControl=SpriteManager->Add(Texture);UpControl->SetColor(D3DCOLOR_XRGB(255,255,255));
	Slider=SpriteManager->Add(Texture);Slider->SetColor(D3DCOLOR_XRGB(255,255,255));

																																

	Strip->SetUniqueMapping(FileOp.ReadVectorFromFile("StripMSLU"),FileOp.ReadVectorFromFile("StripMERD"));
	
	DownControl->SetUniqueMapping(FileOp.ReadVectorFromFile("DownControlMSLU"),FileOp.ReadVectorFromFile("DownControlMERD"));

	UpControl->SetUniqueMapping(FileOp.ReadVectorFromFile("UpControlMSLU"),FileOp.ReadVectorFromFile("UpControlMERD"));


	Slider->SetUniqueMapping(FileOp.ReadVectorFromFile("SliderMSLU"),FileOp.ReadVectorFromFile("SliderMERD"));


					
};


void FNX_GUIScrollBar::Restore()
{
	SpriteManager->Restore();
};

void FNX_GUIScrollBar::ChangeType(bool Vertical)
{
	if(TypeVertical==Vertical)return;
	TypeVertical=Vertical;

	if(Vertical)
	{
		FNX_Vector Temp;
		Temp=Scale;
		Scale.x=Temp.y;Scale.y=Temp.x;
		DownControl->SetRotation(D3DXToRadian(0));Temp=DownControl->GetScale();DownControl->SetScale(Temp.y,Temp.x);
		UpControl->SetRotation(D3DXToRadian(0));Temp=UpControl->GetScale();UpControl->SetScale(Temp.y,Temp.x);
		Slider->SetRotation(D3DXToRadian(0));Temp=Slider->GetScale();Slider->SetScale(Temp.y,Temp.x);
	return;
	};

	if(!Vertical)
	{
		FNX_Vector Temp;
		Temp=Scale;
		Scale.x=Temp.y;Scale.y=Temp.x;
		DownControl->SetRotation(D3DXToRadian(-90));Temp=DownControl->GetScale();DownControl->SetScale(Temp.y,Temp.x);
		UpControl->SetRotation(D3DXToRadian(-90));Temp=UpControl->GetScale();UpControl->SetScale(Temp.y,Temp.x);
		Slider->SetRotation(D3DXToRadian(-90));Temp=Slider->GetScale();Slider->SetScale(Temp.y,Temp.x);
	};

}