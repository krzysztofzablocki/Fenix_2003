#pragma once
/// Sphere shape descriptor
class FNX_SphereShapeDesc : public FNX_ShapeDesc {
public:
	float Radius;    //!< Sphere extents, in local box space

public: 
	//! Constructor
	FNX_SphereShapeDesc() : FNX_ShapeDesc(FNX__SHAPE_SPHERE) {
		Reset();
	}

	//! Setup defaults
	void Reset() {
		Radius=0.0f;
		FNX_ShapeDesc::Reset();
	}

	//! Check if description is valid
	bool isValid() const {
		if (!FNX_ShapeDesc::isValid()) return false;
		if (Radius<= 0.0f) return false;
		return true;
	}  
};
