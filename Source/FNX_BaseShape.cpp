#include "FNX_PhysInc.h"
#include "FNX_BaseShape.h"
#include "FNX_Actor.h"

FNX_BaseShape::FNX_BaseShape(FNX_Actor *actor, const FNX_ShapeDesc &shape)
: Actor(actor)
, Coords(shape.Coords)
{
}

FNX_Coords FNX_BaseShape::GetWorldCoords() {
	return Actor->Coords * Coords;
}

FNX_Vector FNX_BaseShape::GetWorldPos(){
	return GetWorldCoords().T;
}

FNX_Rotation FNX_BaseShape::GetWorldAngle(){
	return GetWorldCoords().R;
}



bool FNX_BaseShape::IsTouchingPoint(const FNX_Vector &point) {
	return false;
}

void FNX_BaseShape::GetInterval_MinMax(const FNX_Vector &axis, float &min, float &max) {
	float center, extents;
	GetInterval_CenExt(axis, center, extents);
	min = center - extents;
	max = center + extents;
}

void FNX_BaseShape::GetInterval_CenExt(const FNX_Vector &axis, float &center, float &extents) {
	float min, max;
	GetInterval_MinMax(axis, min, max);
	center = (max + min) * 0.5f;
	extents = (max - min) * 0.5f;
}