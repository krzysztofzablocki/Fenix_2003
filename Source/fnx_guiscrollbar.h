#pragma once
#include "FNX_GUIBaseElement.h"
#include "FNX_Sprite.h"
#include "FNX_Timer.h"

class FNX_GUIScrollBar : public FNX_GUIBaseElement
{
	protected:
		FNX_Sprite *Strip,*DownControl,*UpControl,*Slider;
		FNX_SpriteManager* SpriteManager;
		bool TypeVertical;///vertical or horizontal?
		int SPosition;///Slider Position
		int Min,Max;///minimal and maximal values
		float VScale;
		FNX_Vector TempPos;
		bool tempD,tempU;
		int JumpCount;
		std::string TextureName;

		FNX_Timer Timer;
		float TT;
	public:
		FNX_GUIScrollBar(FNX_Logger* pLogger,bool IsVertical=true);
		~FNX_GUIScrollBar(){SpriteManager->~FNX_SpriteManager();};
		int GetSPosition(){if(SPosition>Max)SPosition=Max;if(SPosition<Min)SPosition=Min;return SPosition;};///returns Slider Position
		void SetMinMax(FNX_Vector& MM){Min=(int)MM.x;Max=(int)MM.y;	if(SPosition>Max)SPosition=Max;if(SPosition<Min)SPosition=Min;};
		void SetJumpCounter(int val){JumpCount=val;};
		void Render_This();
		bool Update_This(FNX_Input* Input,bool ByMouse=true);
		virtual void LoadFromFile(std::string FileName);
		void Invalidate(){SpriteManager->Invalidate();};
		void Restore();
		void Release(){delete this;};

		void ChangeType(bool Vertical);
		bool IsVertical(){return TypeVertical;};
		void SetSPosition(int v){SPosition=v;};
		int GetMax(){return Max;};
		int GetMin(){return Min;};
	

};