#include "FNX_PhysInc.h"
#include "FNX_Actor.h"
#include "FNX_Synapse.h"
#include "FNX_Constraint.h"

#define SCALAR_TINY ((float) 1.0e-6)
static const float minVelForProcessing = 0.01f;


FNX_Constraint::FNX_Constraint(const FNX_ConstraintDesc &desc)
: LinkA((FNX_Synapse*)desc.linkA)
, LinkB((FNX_Synapse*)desc.linkB)
, Length(desc.Length)
, maxVelMag(desc.maxVelMag)
, Type(desc.Type)
{};

FNX_Constraint::~FNX_Constraint()
{
	if(!LinkB)	LinkB->Release();
	if(!LinkA)	LinkA->Release();
};


void FNX_Constraint::SetLength(float length) {
	if (length < 0.0f) return;
	Length = length;
}

float FNX_Constraint::GetLength() const {
	return Length;
}


FNX_Synapse *FNX_Constraint::GetLinkA() const {
	return LinkA;
}

FNX_Synapse *FNX_Constraint::GetLinkB() const {
	return LinkB;
}




void FNX_Constraint::Update(float timeDelta) {

	if(!LinkA || !LinkB)return;
	if(LinkA->Actor->IsStatic() && LinkB->Actor->IsStatic())return;

	
	if(Type==FT_Constraint_MaxDistance)
	{

		FNX_Vector Pos1=LinkA->GetWorldPosition();
		FNX_Vector Pos2=LinkB->GetWorldPosition();
		FNX_Vector Delta=Pos1-Pos2;
		if(Delta.Length()<Length)return;

		FNX_Actor* This=LinkA->Actor;
		FNX_Actor* Other=LinkB->Actor;

		FNX_Vector CurrentRelPos1=Pos1-Pos2;


		FNX_Vector ra = ~(Pos1- This->GetPosition());
		FNX_Vector rb = ~(Pos2 - Other->GetPosition());
		FNX_Vector Vel1=LinkA->GetWorldVelocity(true);
		FNX_Vector Vel2=LinkB->GetWorldVelocity(true);

		FNX_Vector predRelPos1=CurrentRelPos1+(Vel1-Vel2)*timeDelta;
		FNX_Vector clampedRelPos1=predRelPos1;
		if(clampedRelPos1.Length()<= ((float) 1.0e-6))
			return;
		if(clampedRelPos1.Length()>Length)
			clampedRelPos1*=(Length/clampedRelPos1.Length());

		// now calculate desired vel based on the current pos, new/clamped
		// pos and dt
		FNX_Vector desiredRelVel1=(clampedRelPos1-CurrentRelPos1)/max(timeDelta,SCALAR_TINY);

		// Vr is -ve the total velocity change
		FNX_Vector Vr=(Vel1 - Vel2) - desiredRelVel1;

		float normalVel= Vr.Length();




		if(normalVel>maxVelMag)
		{
			Vr*=(maxVelMag/normalVel);
			normalVel=maxVelMag;
		}
		else
			if (normalVel<minVelForProcessing)
			{
				return;
			};

		FNX_Vector N=Vr/(float)normalVel;
		float m  = This->GetInvMass()+ Other->GetInvMass();
		float ia = This->GetInvInertia() * (ra | N) * (ra | N);
		float ib = Other->GetInvInertia() * (rb | N) * (rb | N);
		float denom = m + ia + ib;

		if(denom<SCALAR_TINY)return;
		float normalImpulse=-normalVel/(float)denom;

		FNX_Vector Pos=(LinkA->GetWorldPosition()+LinkB->GetWorldPosition())*0.5f;

		This->ApplyImpulse(Pos,N*normalImpulse);
		Other->ApplyImpulse(Pos,-N*normalImpulse);
		return;
	};

	if(Type==FT_Constraint_Point)
	{


		FNX_Vector Pos1=LinkA->GetWorldPosition();
		FNX_Vector Pos2=LinkB->GetWorldPosition();
		FNX_Vector Delta=Pos1-Pos2;

		// add a "correction" based on the deviation of point 0
		const FNX_Vector deviation=Pos1 - Pos2;
		float deviationAmount = deviation.Length();
		FNX_Vector mVrExtra;

		float mTimescale=timeDelta*4;

		if (deviationAmount > Length)
		{
			mVrExtra =deviation* ((deviationAmount - Length) / 
				(deviationAmount * max(mTimescale, timeDelta)));
		}
		else
		{
			mVrExtra=FNX_Vector(0);
		};


		FNX_Actor* This=LinkA->Actor;
		FNX_Actor* Other=LinkB->Actor;

		FNX_Vector CurrentRelPos1=Pos1-Pos2;


		FNX_Vector ra = ~(Pos1- This->GetPosition());
		FNX_Vector rb = ~(Pos2 - Other->GetPosition());
		FNX_Vector Vel1=LinkA->GetWorldVelocity(true);
		FNX_Vector Vel2=LinkB->GetWorldVelocity(true);

		FNX_Vector predRelPos1=CurrentRelPos1+(Vel1-Vel2)*timeDelta;
		FNX_Vector clampedRelPos1=predRelPos1;

		// add a "correction" based on the deviation of point 0
		FNX_Vector Vr(mVrExtra + Vel1 - Vel2);

		float normalVel = Vr.Length();

		if (normalVel < minVelForProcessing)
			return ;

		// limit things
		if (normalVel > maxVelMag)
		{
			Vr *= maxVelMag / normalVel;
			normalVel = maxVelMag;
		};


		FNX_Vector N=Vr/(float)normalVel;

		float m  = This->GetInvMass()+ Other->GetInvMass();
		float ia = This->GetInvInertia() * (ra | N) * (ra | N);
		float ib = Other->GetInvInertia() * (rb | N) * (rb | N);
		float denom = m + ia + ib;

		if(denom<SCALAR_TINY)return;
		FNX_Vector normalImpulse=N*(-normalVel/(float)denom);


		FNX_Vector Pos=(LinkA->GetWorldPosition()+LinkB->GetWorldPosition())*0.5f;

		This->ApplyImpulse(Pos,normalImpulse);
		Other->ApplyImpulse(Pos,-normalImpulse);
		return;
	};


	if(Type==FT_Constraint_ConstDistance)//FINE
	{

		FNX_Vector Pos1=LinkA->GetWorldPosition();
		FNX_Vector Pos2=LinkB->GetWorldPosition();
		FNX_Vector Delta=Pos1-Pos2;
		//	if(Delta.Length()==Length)return;

		FNX_Actor* This=LinkA->Actor;
		FNX_Actor* Other=LinkB->Actor;

		FNX_Vector CurrentRelPos1=Pos1-Pos2;


		FNX_Vector ra = ~(Pos1- This->GetPosition());
		FNX_Vector rb = ~(Pos2 - Other->GetPosition());
		//FNX_Vector Vel1=LinkA->GetWorldVelocity();
		//FNX_Vector Vel2=LinkB->GetWorldVelocity();
		FNX_Vector Vel1=LinkA->GetWorldVelocity(true);
		FNX_Vector Vel2=LinkB->GetWorldVelocity(true);

		FNX_Vector predRelPos1=CurrentRelPos1+(Vel1-Vel2)*timeDelta;
		FNX_Vector clampedRelPos1=predRelPos1;
		if(clampedRelPos1.Length()<= ((float) 1.0e-6))
			return;
		if(clampedRelPos1.Length()!=Length)
			clampedRelPos1*=(Length/clampedRelPos1.Length());

		// now calculate desired vel based on the current pos, new/clamped
		// pos and dt
		FNX_Vector desiredRelVel1=(clampedRelPos1-CurrentRelPos1)/max(timeDelta,SCALAR_TINY);

		// Vr is -ve the total velocity change
		FNX_Vector Vr=(Vel1 - Vel2) - desiredRelVel1;

		float normalVel= Vr.Length();




		if(normalVel>maxVelMag)
		{
			Vr*=(maxVelMag/normalVel);
			normalVel=maxVelMag;
		}
		else
			if (normalVel<minVelForProcessing)
			{
				return;
			};

		FNX_Vector N=Vr/(float)normalVel;
		float m  = This->GetInvMass()+ Other->GetInvMass();
		float ia = This->GetInvInertia() * (ra | N) * (ra | N);
		float ib = Other->GetInvInertia() * (rb | N) * (rb | N);
		float denom = m + ia + ib;

		if(denom<SCALAR_TINY)return;
		float normalImpulse=-normalVel/(float)denom;

		FNX_Vector Pos=(LinkA->GetWorldPosition()+LinkB->GetWorldPosition())*0.5f;



		This->ApplyImpulse(Pos,N*normalImpulse);
		Other->ApplyImpulse(Pos,-N*normalImpulse);


		return;
	};
};
