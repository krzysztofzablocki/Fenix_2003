#pragma once
#include "FNX_PhysClasses.h"

/// Simple linear spring
class FNX_Spring{
public:

	//! Synapses
	FNX_Synapse *LinkA, *LinkB;

	//! Stiffness
	float Stiffness;

	//! Damping
	float Damping;

	//! Rest length
	float RestLength;

public:
	//! Constructor
	FNX_Spring(const FNX_SpringDesc &desc);

	//! Set stiffness
	virtual void SetStiffness(float stiffness);

	//! Get stiffness
	virtual float GetStiffness() const;

	//! Set damping
	virtual void SetDamping(float damping);

	//! Get damping
	virtual float GetDamping() const;

	//! Set rest length
	virtual void SetRestLength(float length);

	//! Get rest length
	virtual float GetRestLength() const;  

	//! Get current spring length
	virtual float GetLength() const;

	//! Get first link
	virtual FNX_Synapse *GetLinkA() const;

	//! Get second link
	virtual FNX_Synapse *GetLinkB() const;

	//! Update
	void Update(float timeDelta);
 
};