#pragma once
/// Box shape descriptor
class FNX_BoxShapeDesc : public FNX_ShapeDesc {
public:
	FNX_Vector Extents;    //!< Box extents, in local box space

public: 
	//! Constructor
	FNX_BoxShapeDesc() : FNX_ShapeDesc(FNX__SHAPE_BOX) {
		Reset();
	}

	//! Setup defaults
	void Reset() {
		Extents.Set(10, 10);
		FNX_ShapeDesc::Reset();
	}

	//! Check if description is valid
	bool isValid() const {
		if (!FNX_ShapeDesc::isValid()) return false;
		if (Extents.x <= 0.0f || Extents.y <= 0.0f) return false;
		return true;
	}  
};
