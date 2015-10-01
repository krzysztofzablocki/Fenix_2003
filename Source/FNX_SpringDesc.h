#pragma once
/// Spring description
class FNX_SpringDesc {
public:
	FNX_Synapse *linkA;       //!< First spring link
	FNX_Synapse *linkB;       //!< Second spring link
	float RestLength;       //!< Spring rest length
	float Stiffness;        //!< Spring stiffness
	float Damping;          //!< Spring damping

	//! Constructor
	FNX_SpringDesc() { Reset(); }

	//! Setup defaults
	void Reset() {
		linkA = NULL;
		linkB = NULL;
		RestLength = 0.0f;
		Stiffness = 1.0f;
		Damping = 1.0f;   
	}

	//! Check if description is valid
	bool isValid() const {
	//	if (!linkA || !linkB) return false;
		if (RestLength < 0.0f) return false;
		if (Stiffness < 0.0f) return false;
		if (Damping < 0.0f) return false;
		return true;
	}  
};
