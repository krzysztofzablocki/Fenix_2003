#pragma once


enum FNX_ConstraintType{
	FT_Constraint_MaxDistance=0,
	FT_Constraint_Point=1,
	FT_Constraint_ConstDistance=2
};

class FNX_ConstraintDesc {
public:
	FNX_Synapse *linkA;       /// First Constraint link
	FNX_Synapse *linkB;       /// Second Constraint link
	float Length;       /// Constraint length
	float maxVelMag;///Max Velocity Magnitude
	FNX_ConstraintType Type;
	/// Constructor
	FNX_ConstraintDesc() { Reset(); }

	/// Setup defaults
	void Reset() {
		linkA = 0;
		linkB = 0;
		Length = 0.0f;
		maxVelMag=200.0f;
	}

	/// Check if description is valid
	bool isValid() const {
		if (!linkA || !linkB) return false;
		if (Length < 0.0f) return false;
		if(maxVelMag<0.0f)return false;
		return true;
	}  
};