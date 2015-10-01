#pragma once
/// Actor description
class FNX_ActorDesc {
public:
	vector<FNX_ShapeDesc*> Shapes; //!< Actor shapes
	FNX_Coords Coords;             //!< Actor location
	bool bStatic;                //!< Actor is static
	float Mass;                  //!< Actor mass
	float Inertia;               //!< Actor inertia
	FNX_Actor* RagDollParent;

public: 
	//! Constructor
	FNX_ActorDesc() { Reset(); }

	//! Setup defaults
	void Reset() {
		Coords.Identity();
		Mass = 1.0f;
		Inertia = 1.0f;
		bStatic = false;
		RagDollParent=NULL;
	}

	//! Check if description is valid
	bool isValid() const {
		if (Mass <= 0.0f || Inertia <= 0.0f) return false;
		return true;
	}
};
