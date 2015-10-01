#pragma once
// Shape
#include "FNX_BaseShape.h"

/// Sphere shape
class FNX_SphereShape : public FNX_BaseShape {
public:
	//! Radius
	float Radius;

	//! Cached collider data
	FNX_Vector Center, Axis[4];  


	//! Constructor
	FNX_SphereShape(FNX_Actor *actor, const FNX_SphereShapeDesc &shape);

	//! Is this actor touching given point
	virtual bool IsTouchingPoint(const FNX_Vector &point);   


	//! Update shape
	virtual void Update();

	virtual void UpdateForAxisTest(FNX_BaseShape* BoxShape);

	//! Get number of edges
	virtual int GetNumEdges();

	virtual FNX_ShapeType GetType(){return FNX__SHAPE_SPHERE;};

	//! Get n-th edge
	virtual FNX_Vector GetEdgeDir(int i);

	//! Get SAT interval
	virtual void GetInterval_MinMax(const FNX_Vector &axis, float &min, float &max);

	//! Get SAT interval
	virtual void GetInterval_CenExt(const FNX_Vector &axis, float &center, float &extents);   

	//! Get support vertices for contact generation
	virtual int GetSupportVertices(const FNX_Vector &axis, FNX_Vector *vertices);  

	virtual FNX_Vector GetSize();
};