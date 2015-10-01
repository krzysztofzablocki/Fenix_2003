#pragma once
#include "FNX_PhysClasses.h"


/// Base shape
class FNX_BaseShape {
public:
	//! Owner
	FNX_Actor *Actor;

	//! Local coordinates
	FNX_Coords Coords;

	//! Last bounding box
	FNX_Box WorldBox;

	//! Constructor
	FNX_BaseShape(FNX_Actor *actor, const FNX_ShapeDesc &shape);

	//! Calculat world coordinates
	FNX_Coords GetWorldCoords(); 
	FNX_Vector GetWorldPos();
	FNX_Rotation GetWorldAngle();

	virtual FNX_Vector GetSize()=0;
	//! Is this actor touching given point
	virtual bool IsTouchingPoint(const FNX_Vector &point);  
	virtual FNX_ShapeType GetType()=0;
	//! Update shape
	virtual void Update()=0;

	//! Get number of edges
	virtual int GetNumEdges()=0;

	//! Get n-th edge
	virtual FNX_Vector GetEdgeDir(int i)=0;

	//! Get SAT interval
	virtual void GetInterval_MinMax(const FNX_Vector &axis, float &min, float &max);

	//! Get SAT interval
	virtual void GetInterval_CenExt(const FNX_Vector &axis, float &center, float &extents);  

	//! Get support vertices for contact generation
	virtual int GetSupportVertices(const FNX_Vector &axis, FNX_Vector *vertices)=0;  

};