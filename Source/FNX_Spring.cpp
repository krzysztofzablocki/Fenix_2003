
#include "FNX_PhysInc.h"
#include "FNX_Actor.h"
#include "FNX_Synapse.h"
#include "FNX_Spring.h"

FNX_Spring::FNX_Spring( const FNX_SpringDesc &desc)
: LinkA((FNX_Synapse*)desc.linkA)
, LinkB((FNX_Synapse*)desc.linkB)
, Stiffness(desc.Stiffness)
, Damping(desc.Damping)
, RestLength(desc.RestLength)
{
}


void FNX_Spring::SetStiffness(float stiffness) {
	if (stiffness < 0.0f) return;
	Stiffness = stiffness;
}

float FNX_Spring::GetStiffness() const {
	return Stiffness;
}

void FNX_Spring::SetDamping(float damping) {
	if (damping < 0.0f) return;
	Damping = damping; 
}

float FNX_Spring::GetDamping() const {
	return Damping;
}

void FNX_Spring::SetRestLength(float length) {
	if (length < 0.0f) return;
	RestLength = length;
}

float FNX_Spring::GetRestLength() const {
	return RestLength;
}

float FNX_Spring::GetLength() const {
	FNX_Vector posA = LinkA->GetWorldPosition();
	FNX_Vector posB = LinkB->GetWorldPosition();
	return posA.Distance(posB);
}

FNX_Synapse *FNX_Spring::GetLinkA() const {
	return LinkA;
}

FNX_Synapse *FNX_Spring::GetLinkB() const {
	return LinkB;
}

void FNX_Spring::Update(float timeDelta) {
	// Calculate position
	FNX_Vector posA = LinkA->GetWorldPosition();
	FNX_Vector posB = LinkB->GetWorldPosition();

	// Calculate velocivies
	FNX_Vector velA = LinkA->GetWorldVelocity();
	FNX_Vector velB = LinkB->GetWorldVelocity();

	// Get length
	FNX_Vector dir = posB - posA;
	float length = dir.Normalize();
	if (length < 0.0001f) return;

	// Calculate spring force
	float rellength = length - RestLength;
	FNX_Vector spring = -dir * rellength * Stiffness;

	// Calculate relative velocity and damping
	FNX_Vector relvel = velB - velA;
	FNX_Vector damping = dir * -Damping * (relvel | dir);

	// Calculate final force
	FNX_Vector force = spring + damping; 

	// Apply force
	LinkA->ApplyForce(-force);
	LinkB->ApplyForce(force);
}
