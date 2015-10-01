#include "FNX_PhysClasses.h"
#include "FNX_Actor.h"
#include "FNX_Synapse.h"
#include "FNX_BoxShape.h"
#include "FNX_CollisionSolver.h"

#define MaxVelocityRange (250.0f)
#define MaxAngularVelocityRange (60.0f)

FNX_Actor::FNX_Actor(const FNX_ActorDesc &desc)
: Coords(desc.Coords)
, Velocity(0)
, AngVelocity(0)
, TotalForce(0)
, TotalTorque(0)
, bStatic(desc.bStatic)
, LinImpulse(0)
, AngImpulse(0)
, RagDollParent(desc.RagDollParent)
{

	TempAngVelocity=0.0f;
	TempVelocity=FNX_Vector(0);

	// Setup mass and intertia
	if (bStatic) {
		SetMass(FNX_Infinity);
		SetInertia(FNX_Infinity);
	} else {
		SetMass(desc.Mass);
		SetInertia(desc.Inertia);
	}

	// Add shapes
	for (int i=0; i<(int)desc.Shapes.size(); i++) {
		FNX_ShapeDesc *shapedesc = desc.Shapes[i];
		FNX_BaseShape *shape = NULL;

		// Create shape
		if (shapedesc->nType == FNX__SHAPE_BOX) {
			const FNX_BoxShapeDesc &desc = *(const FNX_BoxShapeDesc*)shapedesc;
			if (desc.isValid()) shape = new FNX_BoxShape(this, desc);
		}

		if (shapedesc->nType == FNX__SHAPE_LINE) {
			const FNX_LineShapeDesc &desc = *(const FNX_LineShapeDesc*)shapedesc;
			if (desc.isValid()) shape = new FNX_BoxShape(this, desc);
		}
/*
		if (shapedesc->nType == FNX__SHAPE_SPHERE) {
			const FNX_SphereShapeDesc &desc = *(const FNX_SphereShapeDesc*)shapedesc;
			if (desc.isValid()) shape = new FNX_SphereShape(this, desc);
		}
*/
		// Add to list of shapes
		if (shape) Shapes.push_back(shape);
	}


	// Update shapes
	UpdateShapes(); 
}

FNX_Actor::~FNX_Actor() {
	// Release shapes
	for (int i=0; i<(int)Shapes.size(); i++) {
		delete Shapes[i];
	}

	// Release synapses
	for (int i=0; i<(int)Synapses.size(); i++) {
		delete Synapses[i];
	}
}


void FNX_Actor::AddShape(FNX_ShapeDesc* Shape)
{
	
		FNX_ShapeDesc *shapedesc = Shape;
		FNX_BaseShape *shape = NULL;

		// Create shape
		if (shapedesc->nType == FNX__SHAPE_BOX) {
			const FNX_BoxShapeDesc &desc = *(const FNX_BoxShapeDesc*)shapedesc;
			if (desc.isValid()) shape = new FNX_BoxShape(this, desc);
		}

		if (shapedesc->nType == FNX__SHAPE_LINE) {
			const FNX_LineShapeDesc &desc = *(const FNX_LineShapeDesc*)shapedesc;
			if (desc.isValid()) shape = new FNX_BoxShape(this, desc);
		}
/*
		if (shapedesc->nType == FNX__SHAPE_SPHERE) {
			const FNX_SphereShapeDesc &desc = *(const FNX_SphereShapeDesc*)shapedesc;
			if (desc.isValid()) shape = new FNX_SphereShape(this, desc);
		}
*/
		// Add to list of shapes
		if (shape) Shapes.push_back(shape);

		UpdateShapes();
};



bool FNX_Actor::IsStatic() const {
	return bStatic;
}


void FNX_Actor::SetMass(float aMass) {
	if (aMass <= 0.0f) return;
	InvMass = (aMass == FNX_Infinity) ? 0.0f : (1.0f / aMass);
	Mass = aMass;
}

inline float FNX_Actor::GetMass() {
	return Mass;
}

void FNX_Actor::SetInertia(float aInertia) {
	if (aInertia <= 0.0f) return;
	InvInertia = (aInertia == FNX_Infinity) ? 0.0f : (1.0f / aInertia); 
	Inertia = aInertia;
}

inline float FNX_Actor::GetInertia() {
	return Inertia;
}

void FNX_Actor::SetLocation(const FNX_Coords &coords) {
	Coords = coords;
}

FNX_Coords FNX_Actor::GetLocation() {
	return Coords;
}

inline void FNX_Actor::SetVelocity(const FNX_Vector &velocity) {
	if (bStatic) return;
	Velocity = velocity;
}

FNX_Vector FNX_Actor::GetVelocity() {
	if (bStatic) return FNX_Vector(0);
	return Velocity;
}

inline void FNX_Actor::SetAngularVelocity(float angvel) {
	if (bStatic) return;
	AngVelocity = angvel;
}

float FNX_Actor::GetAngularVelocity() {
	if (bStatic) return 0.0f;
	return AngVelocity;
}

void FNX_Actor::AddVelocity(const FNX_Vector &velocity) {
	if (bStatic) return;
	Velocity += velocity;
}

void FNX_Actor::AddAngularVelocity(float angvel) {
	if (bStatic) return;
	AngVelocity += angvel;
}

void FNX_Actor::Update(float timeDelta) {
	// Static objects does not need an update
	if (bStatic) return;

	// Apply impulses
	ApplyImpulses();

	// Calculate acceleration
	FNX_Vector Acceleration = TotalForce * InvMass;
	float AngAcceleration = TotalTorque * InvInertia;

	// Integrate acceleration
	Velocity += Acceleration * timeDelta;
	AngVelocity += AngAcceleration * timeDelta;
	
/*	//limit from silly values
	FNX_Limit(Velocity.x,-MaxVelocityRange,MaxVelocityRange);
	FNX_Limit(Velocity.y,-MaxVelocityRange,MaxVelocityRange);
	FNX_Limit(AngVelocity,-MaxAngularVelocityRange,MaxAngularVelocityRange);
*/

	TempAngVelocity=AngVelocity;
	TempVelocity=Velocity;

	// Integrate velocity
	Coords.T += Velocity * timeDelta + Acceleration * (timeDelta * timeDelta * 0.5f);
	Coords.R *= AngVelocity * timeDelta;

	// Update shapes
	UpdateShapes();

	// Reset
	TotalForce = Velocity * -0.2f;
	TotalTorque = AngVelocity * -5.0f;
}

void FNX_Actor::UpdateShapes() {
	// Update shapes
	WorldBox.Zero();
	for (int i=0; i<(int)Shapes.size(); i++) {
		FNX_BaseShape *shape = Shapes[i];
		shape->Update();
		WorldBox += shape->WorldBox;
	}
}

FNX_Vector FNX_Actor::ToLocal(const FNX_Vector &world, bool bVector) {
	if (bVector) {
		FNX_Rotation inv = -Coords.R;
		return inv * world;
	} else {
		FNX_Coords inv = -Coords; 
		return inv * world;
	}
}

FNX_Vector FNX_Actor::ToWorld(const FNX_Vector &local, bool bVector) {
	if (bVector) {
		return Coords % local;
	} else {
		return Coords * local;
	}
}

FNX_Vector FNX_Actor::CalcVelocity(const FNX_Vector &local,bool ByTemp) {
	if (bStatic) return FNX_Vector(0);
	if(ByTemp)
	{
		FNX_Vector tangent = ~local * TempAngVelocity;
		return TempVelocity + ToWorld(tangent, true);
	};

	FNX_Vector tangent = ~local * AngVelocity;
	return Velocity + ToWorld(tangent, true);
}

void FNX_Actor::ApplyForceCOM(const FNX_Vector &force) {
	if (bStatic) return;
	TotalForce += force;
}

void FNX_Actor::ApplyForceWorld(const FNX_Vector &force, const FNX_Vector &pos) {
	if (bStatic) return; 
	TotalForce += force;
	TotalTorque += ToLocal(force, true) | ~ToLocal(pos, false);
}

void FNX_Actor::ApplyForceLocal(const FNX_Vector &force, const FNX_Vector &pos) {
	if (bStatic) return;
	TotalForce += ToWorld(force, true);
	TotalTorque += force | ~pos;
}

void FNX_Actor::SimulateImpulse(const FNX_Vector &pos, const FNX_Vector &impulse) {
	if (bStatic) return;
	float	AngImpulse = impulse | ~(pos - Coords.T);
	TempVelocity += impulse * InvMass;
	TempAngVelocity+= AngImpulse * InvInertia;
}

void FNX_Actor::ApplyImpulse(const FNX_Vector &pos, const FNX_Vector &impulse) {
	if (bStatic) return;

	// Accumulate impulses
	LinImpulse += impulse;
	AngImpulse += impulse | ~(pos - Coords.T);

	SimulateImpulse(pos,impulse);
}

void FNX_Actor::ApplyImpulses() {
	// Apply impulses
	Velocity += LinImpulse * InvMass;
	AngVelocity += AngImpulse * InvInertia;

	// Zero impulses
	LinImpulse.Zero();
	AngImpulse = 0.0f;
}

bool FNX_Actor::IsTouchingPoint(const FNX_Vector &point) {
	// Check if any of the shapes is touching given point
	for (int i=0; i<(int)Shapes.size(); i++) {
		if (Shapes[i]->IsTouchingPoint(point)) return true;
	}

	// Actor is not touching given point
	return false;
}

FNX_Synapse *FNX_Actor::CreateSynapse(const FNX_Vector &position) {
	// Create synapse
	FNX_Synapse *synapse = new FNX_Synapse(this, position);

	// Add to list
	Synapses.push_back(synapse);

	// Return
	return synapse;
}

void FNX_Actor::ReleaseSynapse(FNX_Synapse *pSynapse) {
	if (pSynapse) {
		Synapses.erase(find(Synapses.begin(), Synapses.end(), pSynapse));
		delete pSynapse;
	}
}

void FNX_Actor::PrepareCollision(float timeDelta) {
	CollisionBox = WorldBox;
	FNX_Box moveProj = WorldBox;
	if (!moveProj.isValid) return;
	moveProj.min += Velocity * timeDelta;
	moveProj.max += Velocity * timeDelta;
	CollisionBox += moveProj;
	CollisionBox.Extend(20.0f);
}

void FNX_Actor::ProcessCollision(FNX_Actor *other, float timeDelta) {
	FNX_CollisionSolver solver(Velocity, other->Velocity, timeDelta, timeDelta);

	// Test bodies
	for (int i=0; i<(int)Shapes.size(); i++) {
		for (int j=0; j<(int)other->Shapes.size(); j++) {
			solver.TestShapes(Shapes[i], other->Shapes[j]);
		}
	}

	// No collision
	const FNX_CollisionReport &report = solver.GetReport();
	if (!report.NumContacts) return;

	//   addLine(Coords.T, other->Coords.T);

	// Move actors to collision time
	MoveToCollisionTime(this, other, report);

	// Process contacts
	for (int i=0; i<report.NumContacts; i++) {
		const FNX_Contact &contact = report.Contacts[i];
		ProcessCollisionImpulse(this, other, contact.PositionA, contact.PositionB, -contact.Normal, contact.Scale);
	}

	// Process overlap
	ProcessCollisionOverlap(this, other, report);
}

void FNX_Actor::MoveToCollisionTime(FNX_Actor *actorA, FNX_Actor *actorB, const FNX_CollisionReport &report) {
	actorA->Coords.T += actorA->Velocity * report.Time;
	actorB->Coords.T += actorB->Velocity * report.Time;
}

void FNX_Actor::ProcessCollisionOverlap(FNX_Actor *actorA, FNX_Actor *actorB, const FNX_CollisionReport &report) {
	// No MTD...
	if (report.MTD.SquareLength() < 0.001f) return;

	// Apply MTD
	const float damping = 0.5f;
	if (actorA->InvMass) actorA->Coords.T -= report.MTD * damping;
	if (actorB->InvMass) actorB->Coords.T += report.MTD * damping;
}

void FNX_Actor::ProcessCollisionImpulse(FNX_Actor *actorA, FNX_Actor *actorB, const FNX_Vector &posA, const FNX_Vector &posB, const FNX_Vector &normal, float scale) {
	// Calculate velocivies

	float Elasticity=0.4f;

	FNX_Vector ra = ~(posA - actorA->Coords.T);
	FNX_Vector va = actorA->Velocity + (ra * actorA->AngVelocity);
	FNX_Vector rb = ~(posB - actorB->Coords.T);
	FNX_Vector vb = actorB->Velocity + (rb * actorB->AngVelocity);

	// Caculate relative velocity
	FNX_Vector relvel = va - vb;
	float veldot = relvel | normal;
	if (veldot >= 0.0f) return; // bodies are separating

	// Calculate impulse	
	float numer = -(1 + Elasticity) * veldot;
	float m  = actorA->InvMass + actorB->InvMass;
	float ia = actorA->InvInertia * (ra | normal) * (ra | normal);
	float ib = actorB->InvInertia * (rb | normal) * (rb | normal);
	float denom = m + ia + ib;
	float j = numer / denom;

	// Apply collision impulse
	FNX_Vector impulse = normal * j * scale;
	actorA->ApplyImpulse(posA, impulse);
	actorB->ApplyImpulse(posB, -impulse);

	// Recalculate velocicies since they changed
	va = actorA->Velocity + (ra * actorA->AngVelocity);
	vb = actorB->Velocity + (rb * actorB->AngVelocity);
	relvel = va - vb;

	// Calculate tangent velocity
	FNX_Vector tanvel = relvel - normal * (relvel | normal);
	float tanspeed = tanvel.Length();
	if (tanspeed <= 0.0f) return; // No friction

	// Calculate normalized friction vector
	FNX_Vector T = -tanvel / tanspeed;
	numer = tanspeed;

	// Calculate denominator
	m  = actorA->InvMass + actorB->InvMass;
	ia = actorA->InvInertia * (ra | T) * (ra | T);
	ib = actorB->InvInertia * (rb | T) * (rb | T);
	denom = m + ia + ib;

	// No friction
	if (denom <= 0.0f) return;

	// Calculate friction impulses
	float impuse_to_revers = numer / denom;
	float static_friction = 0.25f;
	float dynamic_friction = 0.25f;
	float impulse_force_normal = static_friction * j;
	float friction_impulse;

	// Decide if we should use static or dynamic friction
	if (impuse_to_revers < impulse_force_normal) {
		friction_impulse = impuse_to_revers;
	} else {
		friction_impulse = dynamic_friction * j;
	}

	// Apply friction impulse
	impulse = T * friction_impulse;
	actorA->ApplyImpulse(posA, impulse);
	actorB->ApplyImpulse(posB, -impulse);
}

