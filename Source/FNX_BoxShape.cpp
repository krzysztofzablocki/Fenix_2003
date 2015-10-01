#include "FNX_PhysInc.h"
#include "FNX_BaseShape.h"
#include "FNX_BoxShape.h"


FNX_BoxShape::FNX_BoxShape(FNX_Actor *actor, const FNX_BoxShapeDesc &desc)
: FNX_BaseShape(actor, desc)
, Extents(desc.Extents)
{
}

FNX_BoxShape::FNX_BoxShape(FNX_Actor *actor, const FNX_LineShapeDesc &desc)
: FNX_BaseShape(actor, desc)
{
	
	int Dir=1;
	if(desc.Start.y>desc.End.y)Dir=-1;

	FNX_Vector v1,v2;
	v1=FNX_Vector(Dir,0);
	v2=desc.End-desc.Start;
	v1.Normalize();v2.Normalize();
	float dot=v1 | v2;

	Coords.T=desc.Start+(desc.End-desc.Start)/2.0f;
	Coords.R=FNX_Rotation(acosf(dot));
	Extents.x=(desc.End-desc.Start).Length()/2.0f;
	Extents.y=desc.BrushSize;
};

bool FNX_BoxShape::IsTouchingPoint(const FNX_Vector &point) {
	FNX_Coords inv_world = -GetWorldCoords();
	FNX_Vector local = inv_world * point;
	if (local.x < -Extents.x || local.x > Extents.x) return false;
	if (local.y < -Extents.y || local.y > Extents.y) return false;
	return true;
}

inline FNX_Vector FNX_BoxShape::GetSize(){
	return Extents;
}

void FNX_BoxShape::Update() {
	// Update center and axis vectors
	FNX_Coords coords = GetWorldCoords(); 
	Center = coords.T;
	Axis[0] = coords.R.AxisX();
	Axis[1] = coords.R.AxisY();

	// Update shape bounding box
	FNX_Vector min, max;
	GetInterval_MinMax(FNX_Vector(1,0), min.x, max.x);
	GetInterval_MinMax(FNX_Vector(0,1), min.y, max.y);
	WorldBox.Set(min, max);
}

int FNX_BoxShape::GetNumEdges() {
	return 2;
}

FNX_Vector FNX_BoxShape::GetEdgeDir(int i) {
	return Axis[i];
}

void FNX_BoxShape::GetInterval_CenExt(const FNX_Vector &axis, float &center, float &extents) {
	center = Center | axis;
	extents = abs(Axis[0] | axis) * Extents.x;
	extents += abs(Axis[1] | axis) * Extents.y;
}

int FNX_BoxShape::GetSupportVertices(const FNX_Vector &axis, FNX_Vector *vertices) {
	const float EdgeEpsilon = 2.6E-3f;
	float dots[2];

	// Calculate dots
	for (int i=0; i<2; i++) {
		dots[i] = axis | Axis[i];
	} 

	// Test edges
	for (int i=0; i<2; i++) {
		// Check if edge is aligned
		if (FNX_Abs(dots[i]) > EdgeEpsilon) continue; // It's not

		// Get edge center
		int crossAxis = (i+1)%2;
		FNX_Vector C = Center - (Axis[crossAxis] * FNX_Sign(dots[crossAxis]) * Extents[crossAxis]);

		// Finalize edge
		vertices[0] = C - Axis[i] * Extents[i];
		vertices[1] = C + Axis[i] * Extents[i];
		return 2;
	}

	// Use vertex
	vertices[0] = Center;
	vertices[0] -= Axis[0] * Extents.x * FNX_Sign(dots[0]);
	vertices[0] -= Axis[1] * Extents.y * FNX_Sign(dots[1]);
	return 1;
}