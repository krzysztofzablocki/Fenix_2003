#pragma once
#include <vector>
#include "FNX_Input.h"
#include "fnx_logger.h"
///Base Element for all gui items

#define BlinkTime 0.9f /// Cursor blink
#define MoveTime  0.1f 
#define MaxClickDelay 0.5f
#define PressTime 0.25f ///Scroll Bar button



enum FNX_GUIEvent
{
	FNX_GUIEvent_OnClick=0,
	FNX_GUIEvent_OnChange=1,
};




class FNX_GUIBaseElement
{
	protected:
		std::vector<FNX_GUIBaseElement*>Elements;
		FNX_Vector Position,Scale;
		FNX_GUIBaseElement* Choosen;
		FNX_Logger* Logger;
		bool None;


///Events functions pointers
	void (*OnPressed)();///pointer to function running when button is pressed
	void (*OnChange)();///pointer to function running when something is changed



		bool Active;
		bool Focus;
	public:
		~FNX_GUIBaseElement();
		FNX_GUIBaseElement();
		virtual bool Update(FNX_Input* Input);
		virtual bool Update_This(FNX_Input* Input,bool ByMouse=true)=0;///must be written in each element returning value identify if it was with action with user...
		virtual void Render();	
		virtual void Render_This()=0;
		virtual void SetScale(FNX_Vector& s){Scale=s;};
		virtual FNX_Vector GetScale(){return Scale;};
		virtual void SetPosition(FNX_Vector& s){Position=s;};
		virtual FNX_Vector GetPosition(){return Position;};
		virtual void LoadFromFile(std::string FileName)=0;
		void Add_Element(FNX_GUIBaseElement* Element){Elements.push_back(Element);};
		virtual void Invalidate()=0;
		virtual void Restore()=0;

		virtual void SetActive(bool v){Active=v;};
		virtual bool GetActive(){return Active;};


		void EventOccured(FNX_GUIEvent Event);
		void SetOnPressedFunction(void (*pOnPressed)()){OnPressed=pOnPressed;};
		void SetOnChangeFunction(void (*pOnChange)()){OnChange=pOnChange;};


		void SetFocus(bool v){Focus=v;};
		virtual bool GetFocus(){return Focus;};///Advanced GUI Elements Like ListBox or ComboBox should make own function


	
		virtual void Release(){};
};
