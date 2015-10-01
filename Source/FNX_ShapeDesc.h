#pragma once
/// Shape type
enum FNX_ShapeType {
	FNX__SHAPE_BOX,
//	FNX__SHAPE_SPHERE,
	FNX__SHAPE_LINE,
};


/// General shape descriptor
class FNX_ShapeDesc {
public: 
	//! Shape type
	FNX_ShapeType nType;

	//! Shape coordinates in local actor space
	FNX_Coords Coords;              

public: 
	//! Constructor
	FNX_ShapeDesc(FNX_ShapeType type) { 
		nType = type;
		Reset();
	}

	//! Setup defaults
	void Reset() {
		Coords.Identity();
	}

	//! Check if description is valid
	bool isValid() const {
		return true;
	}  
};