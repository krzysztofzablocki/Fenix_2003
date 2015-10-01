#include "FNX_GUIListBox.h"
#include "FNX_FileOperations.h"

FNX_GUIListBox::FNX_GUIListBox(FNX_Logger *pLogger)
{
	Logger=pLogger;
	SpriteManager=new FNX_SpriteManager(Logger,100);
	SpriteManager2=new FNX_SpriteManager(Logger,100);
	BackGround=SpriteManager->Add("",false);
	Choosen=SpriteManager2->Add("",false);
	Choosen->SetColor(D3DCOLOR_ARGB(128,0,0,255));
	ChoosenLine=-1;


	ScrollBar= new FNX_GUIScrollBar(Logger);
	Add_Element(static_cast<FNX_GUIBaseElement*>(ScrollBar));

	Font=new FNX_Font(Logger,12,12,false,"Lucida Console");
	
	Scale=FNX_Vector(100,100);
	MultiSelect=HaveChanged=false;
	Color=D3DCOLOR_XRGB(255,0,0);
	TT=0.0f;
};




void FNX_GUIListBox::Invalidate()
{
	SpriteManager->Invalidate();
	SpriteManager2->Invalidate();
	ScrollBar->Invalidate();
	Font->Invalidate();
};



void FNX_GUIListBox::Restore()
{
	SpriteManager->Restore();
	SpriteManager2->Restore();
	ScrollBar->Restore();
	Font->Restore();
};	


/*
bool FNX_GUIListBox::Update_This(FNX_Input* Input,bool ByMouse)
{

	if(ByMouse)
	{
		int sub=((BackGround->GetPosition().y+Scale.y/2+Font->GetHeight()/2)-Input->MousePos.y);
		int s=sub/Font->GetHeight();
		if(s==0)s=1;
	

		if(BackGround->TouchesPoint(Input->MousePos))
			{											

				if(!MultiSelect || (!KEYDOWN(Input->buffer,DIK_LCONTROL) && !KEYDOWN(Input->buffer,DIK_RCONTROL)))
				{
					ChoosenList.clear();
				if(s+ScrollBar->GetSPosition()<=(int)Lines.size())ChoosenList.push_back(ScrollBar->GetSPosition()+s);
					return true;
				}else
					if(!Input->LastFramePressed)
				{

					bool t=false;

					for(int i=0;i<(int)ChoosenList.size();++i)
					{
						if(ChoosenList[i]==s+ScrollBar->GetSPosition())
							{
								t=true;
								ChoosenList.erase(ChoosenList.begin()+i);
								break;
							};

					};

			if(s+ScrollBar->GetSPosition()<=(int)Lines.size())if(!t)ChoosenList.push_back(ScrollBar->GetSPosition()+s);
	
				return true;
				};
	
		return true;

		};


	if(Input->LastFramePressed && BUTTONDOWN(Input->MouseState,0))
	{
		if(Input->MousePos.y>BackGround->GetPosition().y-BackGround->GetScale().y/2){ScrollBar->SetSPosition(ScrollBar->GetSPosition()-1);s=ScrollBar->GetSPosition()+1;};
		if(Input->MousePos.y<BackGround->GetPosition().y+BackGround->GetScale().y/2){ScrollBar->SetSPosition(ScrollBar->GetSPosition()+1);s=ScrollBar->GetSPosition()+Scale.y/Font->GetHeight();};
	
	
		if(!MultiSelect || (MultiSelect && (!KEYDOWN(Input->buffer,DIK_LCONTROL) && !KEYDOWN(Input->buffer,DIK_RCONTROL))))
		{
			ChoosenList.clear();
		if(s<=(int)Lines.size())	ChoosenList.push_back(s);
		};

		return true;
	};


	return false;
};




if(KEYDOWN(Input->buffer,DIK_UPARROW))
{
	if(ChoosenList.size()<=0)return true;

	int s=ChoosenList[0];
	ChoosenList.clear();
	ChoosenList.push_back(s);

	if(ChoosenList[0]>ScrollBar->GetSPosition()+1)ChoosenList[0]--;
	else
	{
		ScrollBar->SetSPosition(ScrollBar->GetSPosition()-1);
		if(ChoosenList[0]>ScrollBar->GetSPosition()+1)ChoosenList[0]--;
		
		if(ChoosenList[0]>(int)Lines.size())ChoosenList[0]=(int)Lines.size();
		if(ChoosenList[0]<0)ChoosenList[0]=0;
	};

return true;
};


if(KEYDOWN(Input->buffer,DIK_DOWNARROW))
{
	if(ChoosenList.size()<=0)return true;
	int s=ChoosenList[0];
	ChoosenList.clear();
	ChoosenList.push_back(s);

	if(ChoosenList[0]<ScrollBar->GetSPosition()+Scale.y/Font->GetHeight()-1)ChoosenList[0]++;
	else
	{
		ScrollBar->SetSPosition(ScrollBar->GetSPosition()+1);
		if(ChoosenList[0]<ScrollBar->GetSPosition()+Scale.y/Font->GetHeight()-1)ChoosenList[0]++;
	
		if(ChoosenList[0]>(int)Lines.size())ChoosenList[0]=(int)Lines.size();
		if(ChoosenList[0]<0)ChoosenList[0]=0;
	};
return true;
};


return false;
};
*/

bool FNX_GUIListBox::Update_This(FNX_Input* Input,bool ByMouse)
{

	int LineCount=(int)Scale.y/Font->GetHeight();//max lines


	if(ByMouse)
	{
		int sub=(int)((BackGround->GetPosition().y+Scale.y/2+Font->GetHeight()/2)-Input->MousePos.y);
		int s=sub/Font->GetHeight();
	//	if(s==0)s=1;
	s-=1;

	
	


		if(BackGround->TouchesPoint(Input->MousePos))
			{	
				if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))
				{
					EventOccured(FNX_GUIEvent_OnClick);
					HaveChanged=true;
				};

				if(s+ScrollBar->GetSPosition()>=(int)Lines.size() || s<0)return true;
				if(s==LineCount)return true;


		
				if(!MultiSelect || (!KEYDOWN(Input->buffer,DIK_LCONTROL) && !KEYDOWN(Input->buffer,DIK_RCONTROL)))
				{
				
							ChoosenList.clear();
							ChoosenList.push_back(ScrollBar->GetSPosition()+s);
							EventOccured(FNX_GUIEvent_OnChange);
							

					return true;
				}else
					if(!Input->LastFramePressed)
				{

					bool t=false;

					for(int i=0;i<(int)ChoosenList.size();++i)
					{
						if(ChoosenList[i]==s+ScrollBar->GetSPosition())
							{
								t=true;
								ChoosenList.erase(ChoosenList.begin()+i);
								break;
							};

					};

			if(s+ScrollBar->GetSPosition()<(int)Lines.size())if(!t)ChoosenList.push_back(ScrollBar->GetSPosition()+s);
			
			EventOccured(FNX_GUIEvent_OnChange);
		

				return true;
				};
	
		return true;

		};


	if(Input->LastFramePressed && BUTTONDOWN(Input->MouseState,0))
	{
		if(Input->MousePos.y>BackGround->GetPosition().y-BackGround->GetScale().y/2){ScrollBar->SetSPosition(ScrollBar->GetSPosition()-1);s=ScrollBar->GetSPosition();};
		if(Input->MousePos.y<BackGround->GetPosition().y+BackGround->GetScale().y/2){ScrollBar->SetSPosition(ScrollBar->GetSPosition()+1);s=ScrollBar->GetSPosition()+(int)Scale.y/Font->GetHeight();};
	
			if(Input->MousePos.y<BackGround->GetPosition().y-BackGround->GetScale().y/2)
				if(Input->MousePos.y>BackGround->GetPosition().y+BackGround->GetScale().y/2)s=-100;
	

		if(!MultiSelect || (MultiSelect && (!KEYDOWN(Input->buffer,DIK_LCONTROL) && !KEYDOWN(Input->buffer,DIK_RCONTROL))))
		{

	if(s!=-100)ChoosenList.clear();
	if(s>=(int)Lines.size())s=(int)Lines.size()-1;	
	if(s>=ScrollBar->GetSPosition()+LineCount)s=ScrollBar->GetSPosition()+LineCount-1;
	if(s!=-100)ChoosenList.push_back(s);
		
		};

		return true;
	};


	return false;
};




if(KEYDOWN(Input->buffer,DIK_UPARROW))
{
	if(ChoosenList.size()<=0)return true;

	int s=ChoosenList[0];
	ChoosenList.clear();
	ChoosenList.push_back(s);

	if(ChoosenList[0]>ScrollBar->GetSPosition())ChoosenList[0]--;
	else
	{
		ScrollBar->SetSPosition(ScrollBar->GetSPosition()-1);
		if(ChoosenList[0]>ScrollBar->GetSPosition())ChoosenList[0]--;
		
		if(ChoosenList[0]>(int)Lines.size())ChoosenList[0]=(int)Lines.size();
		if(ChoosenList[0]<0)ChoosenList[0]=0;
	};

return true;
};


if(KEYDOWN(Input->buffer,DIK_DOWNARROW))
{
	if(ChoosenList.size()<=0)return true;
	int s=ChoosenList[0];
	ChoosenList.clear();
	ChoosenList.push_back(s);


	ChoosenList[0]++;
	if(ChoosenList[0]>ScrollBar->GetSPosition()+LineCount-1)
	{
		ScrollBar->SetSPosition(ScrollBar->GetSPosition()+ChoosenList[0]-(ScrollBar->GetSPosition()+LineCount)+1);
	};

	if(ChoosenList[0]>=(int)Lines.size())ChoosenList[0]=(int)Lines.size()-1;
/*
	if(ChoosenList[0]<ScrollBar->GetSPosition()+LineCount)ChoosenList[0]++;

	if(ChoosenList[0]>ScrollBar->GetSPosition()+LineCount)
	{
		ScrollBar->SetSPosition(ScrollBar->GetSPosition()+1);
		ChoosenList[0]=ScrollBar->GetSPosition()+LineCount-1;
	};

	if(ChoosenList[0]>=(int)Lines.size())ChoosenList[0]=(int)Lines.size()-1;
	if(ChoosenList[0]<0)ChoosenList[0]=0;

*/

return true;
};


return false;
};


void FNX_GUIListBox::Render_This()
{
if(Lines.size()==0)ChoosenList.clear();

	int LineCount=(int)Scale.y/Font->GetHeight()-1;//max lines

	if((int)Lines.size()>LineCount)ScrollBar->SetMinMax(FNX_Vector(0,(float)Lines.size()-LineCount-1));
	else ScrollBar->SetMinMax(FNX_Vector(0,0));



	BackGround->SetScale(Scale.x-20,Scale.y);
	BackGround->SetPosition(Position.x-10,Position.y);

	ScrollBar->SetPosition(FNX_Vector(Position.x+BackGround->GetScale().x/2,Position.y));
	ScrollBar->SetScale(FNX_Vector(20,Scale.y));


	SpriteManager->Render();
	
	std::string Buffer;

	for(int i=ScrollBar->GetSPosition();i<ScrollBar->GetSPosition()+LineCount+1 && i<(int)Lines.size();++i)
	{
	if(i>=0)	Buffer+=Lines[i]+'\n';
	};


	FNX_Vector Poz=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),BackGround->GetPosition().x-BackGround->GetScale().x/2+5,BackGround->GetPosition().y-BackGround->GetScale().y/2);
	FNX_Vector Poz2=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),BackGround->GetPosition().x+BackGround->GetScale().x/2,BackGround->GetPosition().y+BackGround->GetScale().y/2-5);
	
	Font->RenderText(Buffer.c_str(),-1,&FNX_SetRect((LONG)Poz.x,(LONG)Poz2.x,(LONG)Poz.y,(LONG)Poz2.y),DT_LEFT,Color);
	

	Choosen->SetScale(BackGround->GetScale().x,(float)Font->GetHeight());


	for(int i=0;i<(int)ChoosenList.size();++i)
	{
		if((ChoosenList[i]>(ScrollBar->GetSPosition()+LineCount))||(ChoosenList[i]<ScrollBar->GetSPosition()))
		;else
		{
			int s=ChoosenList[i]-ScrollBar->GetSPosition()+1;

		Choosen->SetPosition(BackGround->GetPosition().x,BackGround->GetPosition().y+Scale.y/2- 
			s*Font->GetHeight()+Font->GetHeight()/4.8f);

		SpriteManager2->Render();
		};
	};

	//SpriteManager2->Render();

};



void FNX_GUIListBox::LoadFromFile(std::string FileName)
{
	ScrollBar->LoadFromFile(FileName);

	SpriteManager->~FNX_SpriteManager();
	SpriteManager=new FNX_SpriteManager(Logger,100);
	Font->~FNX_Font();
	Font=NULL;

	FNX_FileOperator FileOp(FileName);

	std::string Texture=FileOp.ReadStringFromFile("Texture");
	BackGround=SpriteManager->Add(Texture);BackGround->SetColor(D3DCOLOR_XRGB(255,255,255));


	BackGround->SetUniqueMapping(FileOp.ReadVectorFromFile("BackGroundMSLU"),FileOp.ReadVectorFromFile("BackGroundMERD"));

	std::string FontName=FileOp.ReadStringFromFile("FontName");
	int h=FileOp.ReadIntFromFile("FontHeight");
	int w=FileOp.ReadIntFromFile("FontWidth");

	if(h==0)h=12;
	if(w==0)w=12;

	Font=new FNX_Font(Logger,h,w,false,FontName.c_str());

	Color=FileOp.ReadColorARGBFromFile("FontColor");

};



std::vector<std::string> FNX_GUIListBox::GetSelectedItemStrings()
{
	std::vector<std::string>re;
	for(int i=0;i<(int)ChoosenList.size();++i)
	{
		re.push_back(Lines[ChoosenList[i]]);
	};


	return re;
};