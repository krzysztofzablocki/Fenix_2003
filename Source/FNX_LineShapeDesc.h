#pragma once
/// Line shape descriptor
class FNX_LineShapeDesc : public FNX_ShapeDesc {
public:
	FNX_Vector Start,End;    //!< Line pos in Local coordinates
	float BrushSize;//!< brush width

public: 
	//! Constructor
	FNX_LineShapeDesc() : FNX_ShapeDesc(FNX__SHAPE_LINE) {
		Reset();
	}

	//! Setup defaults
	void Reset() {
		Start.Zero();End.Zero();
		BrushSize=1.0f;
		FNX_ShapeDesc::Reset();
	}

	//! Check if description is valid
	bool isValid() const {
		if (!FNX_ShapeDesc::isValid()) return false;
		if ( BrushSize<0.0f) return false;
		return true;
	}  
};
