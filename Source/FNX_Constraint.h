#pragma once
#include "FNX_PhysClasses.h"

enum FNX_ConstraintType;

class FNX_Constraint
{
public:
	/// Synapses
	FNX_Synapse *LinkA, *LinkB;

	/// length
	float Length;
	float maxVelMag;///Max Velocity Magnitude
	FNX_ConstraintType Type;
public:
	/// Constructor
	FNX_Constraint(const FNX_ConstraintDesc &desc);
	~FNX_Constraint();
	/// Set stiffness
	/// Set rest length
	virtual void SetLength(float length);

	/// Get rest length
	virtual float GetLength() const;  


	/// Get first link
	virtual FNX_Synapse *GetLinkA() const;

	/// Get second link
	virtual FNX_Synapse *GetLinkB() const;


	/// Update
	void Update(float timeDelta);
};