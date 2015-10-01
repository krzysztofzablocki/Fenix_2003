
#include "FNX_PhysInc.h"
#include "FNX_Synapse.h"
#include "FNX_Actor.h"

FNX_Synapse::FNX_Synapse(FNX_Actor *actor, const FNX_Vector &location)
: Actor(actor)
, Location(location)
{
}

FNX_Actor *FNX_Synapse::GetActor() const {
	return Actor;
}

void FNX_Synapse::Release() {
	if (Actor) {
		Actor->ReleaseSynapse(this);
		Actor = NULL;
	}
}

void FNX_Synapse::ApplyForce(const FNX_Vector &force) {
	Actor->ApplyForceLocal(Actor->ToLocal(force, true), Location);
}

FNX_Vector FNX_Synapse::GetPosition() const {
	return Location;
}

FNX_Vector FNX_Synapse::GetWorldPosition() const {
	return Actor->ToWorld(Location, false);
}

FNX_Vector FNX_Synapse::GetWorldVelocity(bool ByTemp) const {
	return Actor->CalcVelocity(Location,ByTemp);
}
