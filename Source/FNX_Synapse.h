#pragma once
// Classes
#include "FNX_PhysClasses.h"

/// Synapse class implementation
class FNX_Synapse  {
public:
	//! Owning actor
	FNX_Actor *Actor;

	//! Location in actor space
	FNX_Vector Location;

	//! Constructor
	FNX_Synapse(FNX_Actor *actor, const FNX_Vector &location);

	//! Release synapse
	virtual void Release();  

	//! Get actor owning this synapse
	virtual FNX_Actor *GetActor() const;

	//! Apply force at this synapse
	virtual void ApplyForce(const FNX_Vector &force);

	//! Get actor space position of this synapse
	virtual FNX_Vector GetPosition() const;

	//! Get world space position of this synapse
	virtual FNX_Vector GetWorldPosition() const;

	//! Get world space velocity of this synapse
	virtual FNX_Vector GetWorldVelocity(bool ByTemp=false) const;
};
