#include "FNX_PhysicManager.h"
#include "FNX_Synapse.h"

FNX_PhysicManager::~FNX_PhysicManager()
{

	for(int i=0;i<(int)Springs.size();++i)
	{
		Springs[i]->~FNX_Spring();
	};

	for(int i=0;i<(int)Constraints.size();++i)
	{
		Constraints[i]->~FNX_Constraint();
	};

	for(int i=0;i<(int)Actors.size();++i)
	{
		Actors[i]->~FNX_Actor();
	};


}
void FNX_PhysicManager::Update(float dt,FNX_Vector &MousePos,bool LMBUTTONPressed,float StepScale)
{
	if(StepScale<=0.0f)return;//nothing to update
	
	const float minstep = 0.015f*StepScale;
	const float simulationstep=0.015f;

	time_left += dt; 

	while (time_left >= simulationstep) 
	{//update simulation

//	time_left -= minstep;
	time_left -= simulationstep;
	


MouseUpdate(MousePos,LMBUTTONPressed);

for(int i=0;i<(int)Actors.size();++i)
	Actors[i]->PrepareCollision(minstep);


	for(int i=0;i<(int)Actors.size();i++)
		for(int j=i+1;j<(int)Actors.size();j++)
		{
		
		if(Actors[i]->IsStatic() && Actors[j]->IsStatic())continue;//static obiect dont collide 
		if (!Actors[i]->CollisionBox.Touches(Actors[j]->CollisionBox)) continue;	//check BBox
		
		if (Actors[i]->RagDollParent==Actors[j]->RagDollParent && Actors[i]->RagDollParent!=NULL)continue;

			Actors[i]->ProcessCollision(Actors[j],minstep);
		};


	for(int j=0;j<(int)Constraints.size();++j)
	{
		Constraints[j]->Update(minstep);
	};


	for(int j=0;j<(int)Springs.size();++j)
	{
		Springs[j]->Update(minstep);
	};


	for(int i=0;i<(int)Actors.size();++i)
	{
		Actors[i]->Update(minstep);
	};

}
};



void FNX_PhysicManager::AddGlobalForce(FNX_Vector &force,bool dmass)
{
	for(int i=0;i<(int)Actors.size();++i)
	{
		if(!dmass)
			Actors[i]->ApplyForceCOM(force);
		else 
		{
			float tMass=Actors[i]->GetMass();
			if(tMass==FNX_Infinity)tMass=0;
			Actors[i]->ApplyForceCOM(force*tMass);
		};
	}
}


void FNX_PhysicManager::MouseUpdate(FNX_Vector &MousePos, bool LMBUTTONPressed)
{
	if(!MouseDrag)return;


	if(!MouseGhost)
	{
		FNX_ActorDesc D;
		FNX_BoxShapeDesc B;
		B.Extents=FNX_Vector(1.0f,1.0f);
	
		D.bStatic=true;
		D.Shapes.push_back(&B);

		MouseGhost= new FNX_Actor(D);
		MouseAttach=MouseGhost->CreateSynapse(FNX_Vector(0,0));

	};

	if(!MouseSpring)
	{
		FNX_SpringDesc SD;
		SD.Damping=2.0f;SD.RestLength=0.0f;SD.Stiffness=5.0f;
		SD.linkA=MouseAttach;
		MouseSpring= new FNX_Spring(SD);
	
//		MouseSpring->SetLineManager(pLineManager);
	};

	MouseGhost->SetLocation(FNX_Coords(FNX_Rotation(0),MousePos));
	MouseGhost->Update(0.1f);

	if(LMBUTTONPressed)
	{
		if(!LastFramePressed)
		{
		
			Connected=false;
			

			for(int i=0;i<(int)Actors.size();++i)
			{
				if(Actors[i]->IsTouchingPoint(MousePos))
				{
				
					MouseSpring->LinkB=Actors[i]->CreateSynapse(Actors[i]->ToLocal(MousePos));
				
					Connected=true;
				}
			}
		};


		if(Connected)MouseSpring->Update(0.1f);



		LastFramePressed=true;
		return;
	}
	Connected=false;
	LastFramePressed=false;

};


void FNX_PhysicManager::Render()
{
	for(int i=0;i<(int)Springs.size();++i)
	{
		pLineManager->Add(Springs[i]->GetLinkA()->GetWorldPosition(),Springs[i]->GetLinkB()->GetWorldPosition(),D3DCOLOR_XRGB(255,255,0));
	};

if(Connected)pLineManager->Add(MouseSpring->GetLinkA()->GetWorldPosition(),MouseSpring->GetLinkB()->GetWorldPosition(),D3DCOLOR_XRGB(255,255,0));
};