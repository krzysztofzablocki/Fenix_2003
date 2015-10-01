#pragma once
// Shape
#include "FNX_BaseShape.h"

/// Line shape
class FNX_LineShape : public FNX_BaseShape {	///USE IT FOR STATIC OBJECT WHEN YOU SET BRUSH SIZE TO 1 ... 
public:
	//! Extents
	FNX_Vector Extents;

	//! Cached collider data
	FNX_Vector Center, Axis[2];  

public:
	//! Constructor
	FNX_LineShape(FNX_Actor *actor, const FNX_LineShapeDesc &shape);

	//! Is this actor touching given point
	virtual bool IsTouchingPoint(const FNX_Vector &point);   

public:
	//! Update shape
	virtual void Update();

	virtual FNX_ShapeType GetType(){return FNX__SHAPE_LINE;};
	//! Get number of edges
	virtual int GetNumEdges();

	//! Get n-th edge
	virtual FNX_Vector GetEdgeDir(int i);

	//! Get SAT interval
	virtual void GetInterval_CenExt(const FNX_Vector &axis, float &center, float &extents);   

	//! Get support vertices for contact generation
	virtual int GetSupportVertices(const FNX_Vector &axis, FNX_Vector *vertices);  

	virtual FNX_Vector GetSize();
};