#include "FNX_PhysInc.h"
#include "FNX_BaseShape.h"
#include "FNX_SphereShape.h"
#include "FNX_Sprite.h"

FNX_SphereShape::FNX_SphereShape(FNX_Actor *actor, const FNX_SphereShapeDesc &desc)
: FNX_BaseShape(actor, desc)
, Radius(desc.Radius)
{
}

bool FNX_SphereShape::IsTouchingPoint(const FNX_Vector &point) {

	FNX_Coords world=GetWorldCoords();
	FNX_Vector s=world.T-point;
	if(s.Length()>Radius)return false;

	return true;
}

inline FNX_Vector FNX_SphereShape::GetSize(){
	return FNX_Vector(Radius,Radius);
}

void FNX_SphereShape::Update() {
	Center = GetWorldCoords().T;
	WorldBox.Set(FNX_Vector(-Radius,-Radius),FNX_Vector(Radius,Radius));
}


int FNX_SphereShape::GetNumEdges() {
	return 4;
}


FNX_Vector FNX_SphereShape::GetEdgeDir(int i) {
//if(i<4)
	return Axis[i];
}



void FNX_SphereShape::UpdateForAxisTest(FNX_BaseShape* BoxShape)
{
	FNX_Vector Vertices[4];
	GetVertexsFromData(BoxShape->Coords.R,BoxShape->Coords.T,BoxShape->GetSize()*2,Vertices);
	
	FNX_Coords coords= GetWorldCoords();
	for(int i=0;i<4;++i)
	{
		Axis[i]=FNX_Vector(1,((Vertices[i].y-coords.T.y)/(float)(Vertices[i].x-coords.T.x))*(1-coords.T.x)+coords.T.y); 
		Axis[i].Normalize();
	};
	

};






void FNX_SphereShape::GetInterval_MinMax(const FNX_Vector &axis, float &min, float &max) {
	float center, extents;
	GetInterval_CenExt(axis, center, extents);
	min = center - extents;
	max = center + extents;
}

void FNX_SphereShape::GetInterval_CenExt(const FNX_Vector &axis, float &center, float &extents)
{
	FNX_Coords temp=GetWorldCoords();

	center = temp.T | axis;
	extents = fabs(FNX_Vector(1,0) | axis) * Radius;
	extents+= fabs(FNX_Vector(0,1) | axis) * Radius;
//	extents=1.0f;
}


int FNX_SphereShape::GetSupportVertices(const FNX_Vector &axis, FNX_Vector *vertices) {
	const float EdgeEpsilon = 2.6E-3f;
	float dots[4];

/*	FNX_Coords temp=GetWorldCoords();
	Axis[0]=temp.R*FNX_Vector(1,0);
	Axis[1]=temp.R*FNX_Vector(0,1);
*/
	// Calculate dots
	for (int i=0; i<2; i++) {
		dots[i] = axis | Axis[i];
	} 

	

	// Use vertex
	vertices[0] = Center;
//	vertices[0] -= FNX_Vector(1,0)*Radius*FNX_Sign(dots[0]);
//	vertices[0] -= FNX_Vector(0,1)*Radius*FNX_Sign(dots[1]);


	return 1;
}