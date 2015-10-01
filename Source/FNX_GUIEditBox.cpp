#include "FNX_GUIEditBox.h"
#include "FNX_FileOperations.h"


void CopyToClipboard(const std::string& text)
  {
    if (OpenClipboard(NULL)) // Otwieramy schowek
    {
      HGLOBAL hGlobal = NULL;
      char *p = NULL;
      hGlobal = GlobalAlloc (GHND, sizeof(char) * text.size() );
 
      if ( hGlobal )
      {
        p = (char*) GlobalLock(hGlobal);
 
        // Kopiujemy do naszego swiezo za-lockowanego bloczku
        CopyMemory(p,text.c_str(),  sizeof(char) * text.size() );
        
        EmptyClipboard (); // Oczyszczamy schowek
        SetClipboardData (CF_TEXT, hGlobal); // Kopiujemy do niego
        GlobalUnlock(hGlobal); // Zamykamy
      }
      CloseClipboard();
    }
  }
  

  std::string GetFromClipboard()
  {
    std::string rs;
 
    if (OpenClipboard(NULL))
    {
      // Sprawdzamy czy schowek przechowuje dane tekstowe
      if (IsClipboardFormatAvailable (CF_TEXT))
      {
        char *p = NULL;
 
        HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
 
        p = (char *) GlobalLock(hGlobal);
        rs += p;
          GlobalUnlock(hGlobal);
      }
    
      CloseClipboard();
    }
 
    return rs;
  }


FNX_GUIEditBox::FNX_GUIEditBox(FNX_Logger* pLogger)
{
	Logger=pLogger;
	FirstVisibleChar=0;
	CursorChar=0;
	Text="Default Text";

	SpriteManager=new FNX_SpriteManager(Logger,100);
	SpriteManager2=new FNX_SpriteManager(Logger,100);
	BackGround=SpriteManager->Add("",false);
	Select=SpriteManager2->Add("",false);

	Font=new FNX_Font(Logger,12,12,false,"Lucida Console");

	Scale.x=200;Scale.y=20;

	TT=TT2=0.0f;
	First=Last=0;
	WasSelected=false;
	Click=false;
	Color=D3DCOLOR_XRGB(255,0,0);
};




bool FNX_GUIEditBox::Update_This(FNX_Input* Input,bool ByMouse)
{
	
int CharsCount=(int)(Scale.x/Font->GetWidth());
char s=Input->GetChar();

if(First<0)First=0;
if(Last<0)Last=0;
if(Last>(int)Text.size())Last=(int)Text.size();
if(First>(int)Text.size())First=(int)Text.size();


	if(ByMouse)
	{
		if(BackGround->TouchesPoint(Input->MousePos))
		{

			if(Input->LastFramePressed && !BUTTONDOWN(Input->MouseState,0))
			{
				EventOccured(FNX_GUIEvent_OnClick);
				if(!Click)
				{
					Click=true;
					ClickTT=Timer.GetTime();
				}else
				{
					if(Timer.GetTime()-ClickTT<MaxClickDelay)
					{
						First=0;Last=(int)Text.size();
						CursorChar=Last;FirstVisibleChar=(int)Text.size()-CharsCount;
						return true;

					};
				Click=false;

				};

			};




			if(BUTTONDOWN(Input->MouseState,0) && !Input->LastFramePressed)
			{
				CursorChar=(int)((Input->MousePos.x-(Position.x-Scale.x/2.0f))/Font->GetWidth()+FirstVisibleChar);
				if(CursorChar-FirstVisibleChar>CharsCount-1)CursorChar=CharsCount-1+FirstVisibleChar;
				if(CursorChar>(int)Text.size())CursorChar=(int)Text.size();
				if(CursorChar-FirstVisibleChar<0)CursorChar=FirstVisibleChar;
				First=CursorChar;
			};

			if(Input->LastFramePressed)
			{
				CursorChar=(int)((Input->MousePos.x-(Position.x-Scale.x/2.0f))/Font->GetWidth()+FirstVisibleChar);
				if(CursorChar-FirstVisibleChar>CharsCount-1)CursorChar=CharsCount-1+FirstVisibleChar;
				if(CursorChar>(int)Text.size())CursorChar=(int)Text.size();
				if(CursorChar-FirstVisibleChar<0)CursorChar=FirstVisibleChar;
				Last=CursorChar;

			};

			return true;
		};

	
		if(Input->LastFramePressed)
		{
			CursorChar=(int)((Input->MousePos.x-(Position.x-Scale.x/2.0f))/Font->GetWidth()+FirstVisibleChar);
			
			if(Input->MousePos.x>Position.x+Scale.x/2.0f)
				if(FirstVisibleChar<(int)Text.size()-CharsCount+1)
					FirstVisibleChar++;

			if(Input->MousePos.x<Position.x-Scale.x/2.0f)
				if(FirstVisibleChar>0)
					FirstVisibleChar--;



			if(CursorChar-FirstVisibleChar>CharsCount-1)CursorChar=CharsCount-1+FirstVisibleChar;
			if(CursorChar>(int)Text.size())CursorChar=(int)Text.size();
			if(CursorChar-FirstVisibleChar<0)CursorChar=FirstVisibleChar;

			Last=CursorChar;
		

			return true;
		};



		return false;
	};







if(KEYDOWN(Input->buffer,DIK_RIGHTARROW))
 {
	if(CursorChar<(int)Text.size())
	{

		if(!KEYDOWN(Input->buffer,DIK_RSHIFT) && !KEYDOWN(Input->buffer,DIK_LSHIFT))
			if(Timer.GetTime()-TT2>MoveTime || !KEYDOWN(Input->oldbuffer,DIK_RIGHTARROW))
			{
				CursorChar++;
				TT2=Timer.GetTime();
				TT=Timer.GetTime()-1.1f*BlinkTime;
				Last=First=FirstVisibleChar;WasSelected=false;
			};

	
		if(KEYDOWN(Input->buffer,DIK_RSHIFT) || KEYDOWN(Input->buffer,DIK_LSHIFT))
		{
			if(Timer.GetTime()-TT2>MoveTime || !KEYDOWN(Input->oldbuffer,DIK_RIGHTARROW))
			{
				if(!WasSelected)
				{
					WasSelected=true;
					First=CursorChar;
					CursorChar++;
					Last=CursorChar;
					TT2=Timer.GetTime();
					TT=Timer.GetTime()-1.1f*BlinkTime;
				}else
				{
					CursorChar++;
					Last=CursorChar;
					TT2=Timer.GetTime();
					TT=Timer.GetTime()-1.1f*BlinkTime;
				};

			  };

		 };
	};


	if(CursorChar-FirstVisibleChar>CharsCount)
	{
		FirstVisibleChar++;
	if(!WasSelected)Last=First=FirstVisibleChar;
	};

	return true;
};


	if(KEYDOWN(Input->buffer,DIK_LEFTARROW))
	{
if(CursorChar>0)
	{
		
		if(!KEYDOWN(Input->buffer,DIK_RSHIFT)&& !KEYDOWN(Input->buffer,DIK_LSHIFT))
			if(Timer.GetTime()-TT2>MoveTime || !KEYDOWN(Input->oldbuffer,DIK_LEFTARROW))
			{
				CursorChar--;
				TT2=Timer.GetTime();
				TT=Timer.GetTime()-1.1f*BlinkTime;
				Last=First=FirstVisibleChar;WasSelected=false;
			};

	
		if(KEYDOWN(Input->buffer,DIK_RSHIFT) || KEYDOWN(Input->buffer,DIK_LSHIFT))
		{
			if(Timer.GetTime()-TT2>MoveTime || !KEYDOWN(Input->oldbuffer,DIK_LEFTARROW))
			{
				if(!WasSelected)
				{
					WasSelected=true;
					First=CursorChar;
					CursorChar--;
					Last=CursorChar;
					TT2=Timer.GetTime();
					TT=Timer.GetTime()-1.1f*BlinkTime;
				}else
				{
					CursorChar--;
					Last=CursorChar;
					TT2=Timer.GetTime();
					TT=Timer.GetTime()-1.1f*BlinkTime;
				};

			};

		};
	};

	if(CursorChar-FirstVisibleChar<0)
	{
		FirstVisibleChar--;
	if(!WasSelected)Last=First=FirstVisibleChar;
	};


		return true;
};



if((KEYDOWN(Input->buffer,DIK_LCONTROL) && KEYDOWN(Input->buffer,DIK_C))||(KEYDOWN(Input->buffer,DIK_RCONTROL) && KEYDOWN(Input->buffer,DIK_C)))
	{
		int Start,End;
		if(First<Last)
		{
			Start=First;
			End=Last;
		}else
		{
			Start=Last;
			End=First;
		};


		std::string tempS;
		for(int i=Start;i<=End;++i)
		if(i>=0 && i<=(int)Text.size())tempS.push_back(Text[i]);

		CopyToClipboard(tempS);

	EventOccured(FNX_GUIEvent_OnChange);
		return true;
	};


if(Last!=First)
	if((KEYDOWN(Input->buffer,DIK_LCONTROL) && KEYDOWN(Input->buffer,DIK_X))||(KEYDOWN(Input->buffer,DIK_RCONTROL) && KEYDOWN(Input->buffer,DIK_X)))
	{
		int Start,End;
		if(First<Last)
		{
			Start=First;
			End=Last;
		}else
		{
			Start=Last;
			End=First;
		};


		std::string tempS;
		for(int i=Start;i<=End;++i)
		if(i>=0 && i<=(int)Text.size())tempS.push_back(Text[i]);


		Text.erase(Text.begin()+Start,Text.begin()+End);


		CursorChar=Start;
		CopyToClipboard(tempS);


	if(FirstVisibleChar>(int)Text.size()-CharsCount)FirstVisibleChar=(int)Text.size()-CharsCount;

		First=Last=FirstVisibleChar;
		WasSelected=false;



		EventOccured(FNX_GUIEvent_OnChange);
		return true;
	};



if((KEYDOWN(Input->buffer,DIK_RCONTROL) && KEYDOWN(Input->buffer,DIK_V) && !(KEYDOWN(Input->oldbuffer,DIK_RCONTROL)&&KEYDOWN(Input->oldbuffer,DIK_V)))||(KEYDOWN(Input->buffer,DIK_LCONTROL) && KEYDOWN(Input->buffer,DIK_V) && !(KEYDOWN(Input->oldbuffer,DIK_LCONTROL)&&KEYDOWN(Input->oldbuffer,DIK_V))))
	{
		int Start,End;
		if(First<Last)
		{
			Start=First;
			End=Last;
		}else
		{
			Start=Last;
			End=First;
		};


		std::string tempS=GetFromClipboard();

		Text.erase(Text.begin()+Start,Text.begin()+End);
		if(Last!=First)
		Text.insert(Text.begin()+Start,tempS.begin(),tempS.end());
		else
		Text.insert(Text.begin()+CursorChar,tempS.begin(),tempS.end());

	First=Last=FirstVisibleChar;


	EventOccured(FNX_GUIEvent_OnChange);
		return true;
	};



if((KEYDOWN(Input->buffer,DIK_LCONTROL) || KEYDOWN(Input->buffer,DIK_RCONTROL)) && KEYDOWN(Input->buffer,DIK_A ))
{
	First=0;Last=(int)Text.size();
	CursorChar=Last;FirstVisibleChar=(int)Text.size()-CharsCount;
	return true;
};


if (KEYDOWN(Input->buffer,DIK_LCONTROL) || KEYDOWN(Input->buffer,DIK_RCONTROL))return false; //ctrl + char not allowed


if(KEYDOWN(Input->buffer,DIK_DELETE))
{
	int Start,End;
					if(First<Last)
						{
							Start=First;			
							End=Last;
						}else
						{	
							Start=Last;
							End=First;
						};	

				

if(Last==First)
{

if(Timer.GetTime()-TT2>MoveTime)
{
	Text.erase(Text.begin()+CursorChar);
	TT2=Timer.GetTime();
};

}else
{
		Text.erase(Text.begin()+Start,Text.begin()+End);
				
		if(FirstVisibleChar>(int)Text.size()-CharsCount)FirstVisibleChar=(int)Text.size()-CharsCount;

		CursorChar=Start;
		First=Last=FirstVisibleChar;
		WasSelected=false;
		TT2=Timer.GetTime();

		EventOccured(FNX_GUIEvent_OnChange);

};

return true;


};

if(s!=0)
{
	
		if(KEYDOWN(Input->buffer,DIK_BACKSPACE))
		{
		
			if(Last==First)
		
			{
			
				if(CursorChar>=1)
					Text.erase(Text.begin()+CursorChar-1);
		
				if(CursorChar>0)CursorChar--;
				if(CursorChar<FirstVisibleChar)FirstVisibleChar--;
		
			}
			else
			{
				int Start,End;
					if(First<Last)
						{
							Start=First;			
							End=Last;
						}else
						{	
							Start=Last;
							End=First;
						};	



		Text.erase(Text.begin()+Start,Text.begin()+End);
				
		if(FirstVisibleChar>(int)Text.size()-CharsCount)FirstVisibleChar=(int)Text.size()-CharsCount;

		CursorChar=Start;
		First=Last=FirstVisibleChar;
		WasSelected=false;

			};

EventOccured(FNX_GUIEvent_OnChange);

		return true;	
		};





if(Last!=First)
{
	int Start,End;
					if(First<Last)
						{
							Start=First;			
							End=Last;
						}else
						{	
							Start=Last;
							End=First;
						};	



		Text.erase(Text.begin()+Start,Text.begin()+End);
				
		if(FirstVisibleChar>(int)Text.size()-CharsCount)FirstVisibleChar=(int)Text.size()-CharsCount;

		CursorChar=Start;
		First=Last=FirstVisibleChar;
		WasSelected=false;
};



	Text.insert(Text.begin()+CursorChar,s);
	CursorChar++;
	if(CursorChar>FirstVisibleChar+CharsCount)
	{
		FirstVisibleChar++;
	//	CursorChar--;
	};



	EventOccured(FNX_GUIEvent_OnChange);

return true;
};





	return false;
};




void FNX_GUIEditBox::Render_This()
{
	if(FirstVisibleChar<0)FirstVisibleChar=0;
	int CharsCount=(int)(Scale.x/Font->GetWidth());

	BackGround->SetColor(D3DCOLOR_XRGB(255,255,255));
	BackGround->SetScale(Scale);
	BackGround->SetPosition(Position);


	int rFirst=First;
	if(First<Last)rFirst=First;else rFirst=Last;

	if(rFirst<FirstVisibleChar)
		rFirst=FirstVisibleChar;
	else rFirst=rFirst;

	int rLast=Last;
	if(First<Last)rLast=Last;else rLast=First;
	rLast=rLast-FirstVisibleChar;
	if(rLast>CharsCount+1)rLast=CharsCount+1;


	Select->SetColor(D3DCOLOR_ARGB(127,0,0,255));
	Select->SetScale(FNX_Vector((rLast-rFirst+FirstVisibleChar)*(float)Font->GetWidth(),Scale.y));
	Select->SetPosition(FNX_Vector(Position.x-Scale.x/2.0f+((rLast-rFirst-FirstVisibleChar)/2.0f+rFirst)*Font->GetWidth(),Position.y));


	SpriteManager->Render();
	SpriteManager2->Render();

	FNX_Vector Poz=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),BackGround->GetPosition().x-BackGround->GetScale().x/2,BackGround->GetPosition().y-BackGround->GetScale().y/4);
	FNX_Vector Poz2=FNX_TranslatePosLHToOffScreenLH(Logger->GethWnd(),BackGround->GetPosition().x+BackGround->GetScale().x/2,BackGround->GetPosition().y+BackGround->GetScale().y/4);
	

	std::string rText;

	for(int i=FirstVisibleChar;i<(int)Text.size();++i)
	{
		rText.push_back(Text[i]);
	};


	Font->RenderText(rText.c_str(),-1,&FNX_SetRect((LONG)Poz.x,(LONG)Poz2.x,(LONG)Poz.y,(LONG)Poz2.y),DT_SINGLELINE,Color);


	


	BackGround->SetColor(D3DCOLOR_XRGB(0,0,255));
	BackGround->SetScale(2,Scale.y);
	BackGround->SetPosition(FNX_Vector(Position.x-Scale.x/2.0f+(CursorChar-FirstVisibleChar)*Font->GetWidth(),Position.y));
	

	if(GetFocus())
	if(Timer.GetTime()-TT>BlinkTime)
	{
		SpriteManager->Render();
	if(Timer.GetTime()-TT>2*BlinkTime)TT=Timer.GetTime();
	};

	BackGround->SetScale(Scale);
	BackGround->SetPosition(Position);

};

void FNX_GUIEditBox::LoadFromFile(std::string FileName)
{
	SpriteManager->Release();
	SpriteManager=new FNX_SpriteManager(Logger,100);

	Font->~FNX_Font();
	Font=NULL;


		FNX_FileOperator FileOp(FileName);
		std::string Texture=FileOp.ReadStringFromFile("Texture");

	BackGround=SpriteManager->Add(Texture,false);
	BackGround->SetUniqueMapping(FileOp.ReadVectorFromFile("BackGroundMSLU"),FileOp.ReadVectorFromFile("BackGroundMERD"));


	std::string FontName=FileOp.ReadStringFromFile("FontName");
	int h=FileOp.ReadIntFromFile("FontHeight");
	int w=FileOp.ReadIntFromFile("FontWidth");

	Font=new FNX_Font(Logger,h,w,false,FontName.c_str());
	Color=FileOp.ReadColorARGBFromFile("FontColor");

};