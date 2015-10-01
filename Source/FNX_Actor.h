#pragma once
// Classes
#include "FNX_PhysClasses.h"
#include "FNX_PhysInc.h"

/// Internal actor
class FNX_Actor {
public:
	FNX_Actor* RagDollParent;
	//! Actor shapes
	vector<FNX_BaseShape*> Shapes;

	//! Actor synapses
	vector<FNX_Synapse*> Synapses;

	//! Actor COM coordinate system
	FNX_Coords Coords;

	//! Actor COM velocity       
	FNX_Vector Velocity;

	//! Actor angular velocity
	float AngVelocity;

	//! Mass
	float Mass, InvMass;

	//! Inertia
	float Inertia, InvInertia;

	//! Linear impulse accumulator
	FNX_Vector LinImpulse;

	//! Angular impulse accumulator
	float AngImpulse;

	//! Force accumulator
	FNX_Vector TotalForce;

	//! Torque accumulator
	float TotalTorque;

	//! Bounding box
	FNX_Box WorldBox;

	//! Collision projection box
	FNX_Box CollisionBox;

	//! Draw color
	DWORD DrawColor;

	//! This is static actor
	bool bStatic;

	FNX_Vector TempVelocity;
	float TempAngVelocity;


	//! Constructor
	FNX_Actor(const FNX_ActorDesc &desc);

	//! Destructor
	~FNX_Actor(); 

	//! Add new Shape to actor
	virtual void AddShape(FNX_ShapeDesc* Shape);
	//! Is this static actor
	virtual bool IsStatic() const;

	//! Set mass
	virtual void SetMass(float Mass);

	//! Get mass
	virtual float GetMass();

	virtual float GetInvMass(){return InvMass;};
	virtual float GetInvInertia(){return InvInertia;};
	//! Set inertia
	virtual void SetInertia(float Inertia);

	//! Get inertia
	virtual float GetInertia();

	//! Set location
	virtual void SetLocation(const FNX_Coords &coords);

	//! Get location
	virtual FNX_Coords GetLocation();
	virtual FNX_Vector GetPosition(){return GetLocation().T;};

	//! Set velocity
	virtual void SetVelocity(const FNX_Vector &velocity);

	//! Get velocity
	virtual FNX_Vector GetVelocity();

	//! Set angular velocity
	virtual void SetAngularVelocity(float angvel);

	//! Get angular velocity
	virtual float GetAngularVelocity();

	//! Convert vector/point from world to local actor space
	virtual FNX_Vector ToLocal(const FNX_Vector &world, bool bVector=false);

	//! Convert vector/point from local actor space to world space
	virtual FNX_Vector ToWorld(const FNX_Vector &local, bool bVector=false);  

	//! Calculate velocity at given point in local actor space, velocity is in world space, we can choose to get simulated Velocity(bytemp=true)
	virtual FNX_Vector CalcVelocity(const FNX_Vector &local,bool ByTemp=false);  

	//! Add velocity
	virtual void AddVelocity(const FNX_Vector &velocity);

	//! Add angular velocity
	virtual void AddAngularVelocity(float angvel);  

	//! Apply force at COM, force is specified in world space
	virtual void ApplyForceCOM(const FNX_Vector &force);

	//! Apply force at given point in world space, force is specified in world space
	virtual void ApplyForceWorld(const FNX_Vector &force, const FNX_Vector &pos);

	//! Apply force at given point in local space, force is specified in local space
	virtual void ApplyForceLocal(const FNX_Vector &force, const FNX_Vector &pos);

	//! Apply collision impulse
	virtual void ApplyImpulse(const FNX_Vector &pos, const FNX_Vector &impulse);

	//! Simulate collision impulse
	virtual void SimulateImpulse(const FNX_Vector &pos, const FNX_Vector &impulse);

	//! Create synapse at given point in actor space
	virtual FNX_Synapse *CreateSynapse(const FNX_Vector &position);

	//! Synapse was destroyed
	void ReleaseSynapse(FNX_Synapse *pSynapse);

	//! Update
	void Update(float timeDelta);

	//! Is this actor touching given point
	bool IsTouchingPoint(const FNX_Vector &point); 

	//! Prepare collision detection
	void PrepareCollision(float timeDelta);

	//! Process collision
	void ProcessCollision(FNX_Actor *other, float timeDelta);

	//! Update shapes
	void UpdateShapes();

	//! Apply collision impulses
	void ApplyImpulses();

	//! Move actors to collision time
	static void MoveToCollisionTime(FNX_Actor *actorA, FNX_Actor *actorB, const FNX_CollisionReport &report);

	//! Process actors overlap
	static void ProcessCollisionOverlap(FNX_Actor *actorA, FNX_Actor *actorB, const FNX_CollisionReport &report);

	//! Process collision impulse
	static void ProcessCollisionImpulse(FNX_Actor *actorA, FNX_Actor *actorB, const FNX_Vector &posA, const FNX_Vector &posB, const FNX_Vector &normal, float scale);  

};