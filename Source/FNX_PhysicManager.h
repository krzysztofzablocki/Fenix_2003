#pragma once
#include "FNX_PhysInc.h"
#include "FNX_Actor.h"
#include "FNX_Spring.h"
#include "FNX_BaseShape.h"
#include "FNX_Constraint.h"

#include "fnx_logger.h"
#include "FNX_LineManager.h"

class FNX_PhysicManager
{
private:
	std::vector<FNX_Actor*>Actors;
	std::vector<FNX_Spring*>Springs;
	std::vector<FNX_Constraint*>Constraints;
	FNX_LineManager* pLineManager;
	FNX_Logger* pLogger;
	float time_left; 

	HWND hWnd;
	bool MouseDrag;
	bool LastFramePressed;
	FNX_Actor* MouseGhost;
	FNX_Synapse* MouseAttach;
	FNX_Spring* MouseSpring;
	bool Connected;
public:

	FNX_PhysicManager(FNX_Logger* L){pLogger=L;pLineManager=pLogger->GetLineManager();hWnd=pLogger->GethWnd();MouseDrag=LastFramePressed=false;MouseSpring=NULL;MouseGhost=NULL;Connected=false;};
    ~FNX_PhysicManager();
	void Release(){delete this;};

	virtual void Add(FNX_Actor* o)
	{
		Actors.push_back(o);
	};

	virtual void Add(FNX_Spring* o)
	{
		Springs.push_back(o);
//		o->SetLineManager(pLineManager);

	};
	
	virtual void Add(FNX_Constraint* o)
	{
		Constraints.push_back(o);
	//	o->SetLineManager(pLineManager);

	};

	virtual void SetMouseDrag(bool v){MouseDrag=v;};
	virtual void MouseUpdate(FNX_Vector &MousePos,bool LMBUTTONPressed);
	virtual void AddGlobalForce(FNX_Vector &force,bool dmass=false);//dmass=true => force*object.mass
	virtual void Update(float dt,FNX_Vector &MousePos,bool LMBUTTONPressed,float StepScale=1.0f);///Scaling Simulation step can be used to slow motion etc. StepScale Range (0.0f,INF)
	
	virtual void Render();
};