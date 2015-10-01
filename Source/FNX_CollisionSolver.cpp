
#include "FNX_PhysInc.h"
#include "FNX_BaseShape.h"
#include "FNX_Actor.h"
#include "FNX_CollisionSolver.h"


FNX_CollisionSolver::FNX_CollisionSolver(const FNX_Vector &velocityA, const FNX_Vector &velocityB, float taMax, float tbMax) {
	VelocityA = velocityA;
	VelocityB = velocityB;
	RelativeVelocity = velocityB - velocityA;
	MaxTime = min(taMax, tbMax);
}

const FNX_CollisionReport &FNX_CollisionSolver::GetReport() const {
	return Report;
}

int FNX_CollisionSolver::TestShapes(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB) {
	// Test SAT
	float ColTime = MaxTime;
	FNX_Vector ColNormal(0);
	FNX_Vector ColMTD(0);
	if (!TestSAT(shapeA, shapeB, ColTime, ColNormal, ColMTD)) return 0; 

	// Generate contacts
	int start = Report.NumContacts;
	if (!CalculateContacts(shapeA, shapeB, ColTime, ColNormal)) return 0;
	if (start == Report.NumContacts) return 0;

	// Draw contacts
	/*for (int i=start; i<Report.NumContacts; i++) {
	const FNX_Contact &C = Report.Contacts[i];  
	addLine(C.PositionA+FNX_Vector(2,2), C.PositionA+FNX_Vector(-2,-2));
	addLine(C.PositionA+FNX_Vector(-2,2), C.PositionA+FNX_Vector(2,-2));
	addLine(C.PositionB+FNX_Vector(2,2), C.PositionB+FNX_Vector(-2,-2));
	addLine(C.PositionB+FNX_Vector(-2,2), C.PositionB+FNX_Vector(2,-2));  
	addLine(C.PositionA, C.PositionA-C.Normal*6.0f);
	addLine(C.PositionB, C.PositionB+C.Normal*6.0f);
	}//*/

	// Update report
	if (start == 0) {
		Report.MTD = ColMTD;
		Report.Time = ColTime;
	} else {
		if (ColTime < Report.Time) {
			Report.Time = ColTime;
		}

		if (ColMTD.SquareLength() > Report.MTD.SquareLength()) {
			Report.MTD = ColMTD;
		}
	} 

	// All ok
	return Report.NumContacts - start;
}

bool FNX_CollisionSolver::TestSAT(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB, float &colTime, FNX_Vector &colNormal, FNX_Vector &colMTD) {
	// Initialize MTD
	FNX_Vector MTD(FNX_Infinity, FNX_Infinity);
	bool bValidMTD = true; 

	// Get number of edges of each body
	int edgesA = shapeA->GetNumEdges();
	int edgesB = shapeB->GetNumEdges();

	// Collision test. Calculate the min and max times of collisions of the objects
	// could be negative (then the data returned will be an intersection MTD data).
	float tfirst = -FNX_Infinity;
	float tlast = FNX_Infinity;
	FNX_Vector nfirst(0);
	FNX_Vector nlast(0);

/*
	if(shapeA->GetType()==FNX__SHAPE_SPHERE || shapeB->GetType()==FNX__SHAPE_SPHERE)
	if(shapeA->GetType()!=FNX__SHAPE_SPHERE || shapeB->GetType()!=FNX__SHAPE_SPHERE )
	{
		if(shapeA->GetType()==FNX__SHAPE_SPHERE)dynamic_cast<FNX_SphereShape*>(shapeA)->UpdateForAxisTest(shapeB);
		if(shapeB->GetType()==FNX__SHAPE_SPHERE)dynamic_cast<FNX_SphereShape*>(shapeB)->UpdateForAxisTest(shapeA);
	};
*/

	// Edges of A
	for (int i=0; i<edgesA; i++) {
		FNX_Vector axis = shapeA->GetEdgeDir(i);
		if (!TestInterval(shapeA, shapeB, axis, MTD, bValidMTD, tfirst, tlast, nfirst, nlast)) return false;
	}

	// Edges of B	
	for (int i=0; i<edgesB; i++) {
		FNX_Vector axis = shapeB->GetEdgeDir(i);
		if (!TestInterval(shapeA, shapeB, axis, MTD, bValidMTD, tfirst, tlast, nfirst, nlast)) return false;
	}





	// Missed  collision ?
	if (tfirst > MaxTime || tlast < 0.0f) return false;

	// Boxes are intersecting (or velocity too small)
	if (tfirst <= 0.0f) {
		// We don't have valid MTD
		if (!bValidMTD) return false;

		// Use MTD as contact point
		colTime = 0.0f;
		colMTD = MTD;
		colNormal = MTD;
		colNormal.Normalize();
	} else {
		colTime = tfirst;
		colNormal = nfirst;
		colMTD.Zero();
	}

	// We have collision !
	return true;
}

bool FNX_CollisionSolver::TestInterval(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB, const FNX_Vector &axis, FNX_Vector &MTD, bool &bValidMTD, float &tfirst, float &tlast, FNX_Vector &nfirst, FNX_Vector &nlast) {
	// Projection calculations
	float minb, maxb, c, e;

	// Both intervals
	shapeA->GetInterval_CenExt(axis, c, e);
	shapeB->GetInterval_MinMax(axis, minb, maxb);

	// Reduce the problem of a single point versus a larger interval
	minb -= e;
	maxb += e;

	// Intersection test calculations
	float d0 = minb - c;
	float d1 = maxb - c; 

	// Are the objects separated along that axis?
	bool bIntersect = d0 < 0.0f && d1 > 0.0f;

	// If so, then we can have no potential interection
	bValidMTD &= bIntersect;

	// If the intersection is still potentially valid
	if (bValidMTD) {
		// Find the MTD along that axis
		FNX_Vector sep = axis;

		// Scale
		if (fabs(d0) < fabs(d1)) {
			sep *= -d0;
		} else {
			sep *= -d1;
		}

		// Better ?
		if (sep.SquareLength() < MTD.SquareLength()) {
			MTD = sep;
		}
	}

	// Collision test calculations
	float v = RelativeVelocity | axis;

	// Boxes virtual never move on that axis. It's worth continuing only if the box are not separated.
	if (fabs(v) < 1.0E-6f)	return bIntersect;

	// Time of intersection along that axis
	float t0 = -(minb - c) / v;
	float t1 = -(maxb - c) / v;
	float sign = -1.0f;

	// Order the times of collision properly
	if (t0 > t1) {
		swap(t0, t1);
		sign = 1.0f;
	}

	// Make sure the axis intersection provides a valid intersection interval
	if (t0 > tlast) return false;
	if (t1 < tfirst) return false;

	// Then reduce the global intersection interval
	if (t1 < tlast) {
		tlast = t1;
		nlast = axis * sign;
	}

	// Reduce...
	if (t0 > tfirst) {
		tfirst = t0;
		nfirst = axis * -sign;
	}

	// Collision on this axis
	return true;
}

bool FNX_CollisionSolver::CalculateContacts(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB, float colTime, const FNX_Vector &colNormal) {
	FNX_Vector SupportA[4], SupportB[4];

	// Get support vertices
	int numA = shapeA->GetSupportVertices(-colNormal, SupportA);
	int numB = shapeB->GetSupportVertices( colNormal, SupportB);

	//addLine(SupportA[0], SupportA[0] + colNormal*50); 

	// Project vertices
	for (int i=0; i<numA; i++) SupportA[i] += VelocityA * colTime;
	for (int i=0; i<numB; i++) SupportB[i] += VelocityB * colTime;

	// Point-Point contact
	if (numA == 1) {
		if (numB == 1) {
			SolvePPContact(SupportA[0], SupportB[0], colNormal);
		} else if (numB == 2) {
			SolvePEContact(SupportA[0], SupportB[0], SupportB[1], colNormal, false);  
		} 
	} else if (numA == 2) {
		if (numB == 1) {
			SolvePEContact(SupportB[0], SupportA[0], SupportA[1], colNormal, true);    
		} else if (numB == 2) {
			SolveEEContact(SupportA[0], SupportA[1], SupportB[0], SupportB[1], colNormal);
		} 
	}

	return true;
}

bool FNX_CollisionSolver::SolvePPContact(const FNX_Vector &posA, const FNX_Vector &posB, const FNX_Vector &normal) {
	Report.AddContact(posA, posB, normal, 1.0f);
//	printf("Point-Point\n");
	return true;
}

bool FNX_CollisionSolver::SolvePEContact(const FNX_Vector &pos, const FNX_Vector &edgeA, const FNX_Vector &edgeB, const FNX_Vector &normal, bool bSwap) {
	FNX_Vector dir = pos - edgeA;
	FNX_Vector edge = edgeB - edgeA;
	float t = (dir | edge) / (edge | edge);
	if (t > 1.0f) t = 1.0f;
	if (t < 0.0f) t = 0.0f;
	FNX_Vector bpos = edgeA + edge * t;
	if (bSwap) {
		Report.AddContact(bpos, pos, normal, 1.0f);
	} else {
		Report.AddContact(pos, bpos, normal, 1.0f);
	}
	return true;
}

bool FNX_CollisionSolver::SolveEEContact(const FNX_Vector &edgeA1, const FNX_Vector &edgeA2, const FNX_Vector &edgeB1, const FNX_Vector &edgeB2, const FNX_Vector &normal) {
	FNX_Vector proj = ~normal;

	// Edge A
	float distA1 = edgeA1 | proj;
	float distA2 = edgeA2 | proj;
	if (distA1 > distA2) swap(distA1, distA2); 

	// Edge B
	float distB1 = edgeB1 | proj;
	float distB2 = edgeB2 | proj;
	if (distB1 > distB2) swap(distB1, distB2); 

	// Merge
	float min = (distA1 > distB1) ? distA1 : distB1;
	float max = (distA2 < distB2) ? distA2 : distB2;
	if (min > max) return false;

	// Generate contacts
	float baseA = edgeA1 | proj;
	FNX_Vector posA1 = edgeA1 + proj * (min - baseA);
	FNX_Vector posA2 = edgeA1 + proj * (max - baseA);
	float baseB = edgeB1 | proj;
	FNX_Vector posB1 = edgeB1 + proj * (min - baseB);
	FNX_Vector posB2 = edgeB1 + proj * (max - baseB);

	/* // blee
	addLine(posA1, posA2);
	addLine(posB1, posB2);*/

	// Add contacts
	Report.AddContact(posA1, posB1, normal, 0.75f);
	Report.AddContact(posA2, posB2, normal, 0.75f); 
//	Report.AddContact(posA1, posB1, normal, 0.5f);
//	Report.AddContact(posA2, posB2, normal, 0.5f); 
	return true;
}