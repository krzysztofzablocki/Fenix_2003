#pragma once


#include "FNX_PhysClasses.h"

/// Collision contact
class FNX_Contact {
public:
	FNX_Vector PositionA;    //!< Contact location on first body
	FNX_Vector PositionB;    //!< Contact location on second body
	FNX_Vector Normal;       //!< Contact normal 
	float Scale;           //!< Impulse scale
};  

/// Collision report
class FNX_CollisionReport {
public:
	enum { MAX_CONTACTS=16 };   //!< Maximum number of contacts

public:
	float Time;                           //!< Collision time
	FNX_Vector MTD;                         //!< Separation vector
	int NumContacts;                      //!< Number of contacts
	FNX_Contact Contacts[MAX_CONTACTS];     //!< Collision contacts 

public:
	//! Constructor
	FNX_CollisionReport() : Time(0.0f), MTD(0), NumContacts(0) {};

	//! Add contact pair
	void AddContact(const FNX_Vector &Apos, const FNX_Vector &Bpos, const FNX_Vector &normal, float scale) {
		if (NumContacts >= MAX_CONTACTS) return;
		Contacts[NumContacts].PositionA = Apos;
		Contacts[NumContacts].PositionB = Bpos;
		Contacts[NumContacts].Normal = normal;
		Contacts[NumContacts].Scale = scale;
		NumContacts++;
	}   
};

/// Collision solver
class FNX_CollisionSolver { 
private:
	//! Bodies velocity
	FNX_Vector VelocityA, VelocityB;

	//! Relative velocity
	FNX_Vector RelativeVelocity;

	//! Collision report
	FNX_CollisionReport Report;

	//! Max collision time
	float MaxTime;

public:
	//! Initialize solver
	FNX_CollisionSolver(const FNX_Vector &velocityA, const FNX_Vector &velocityB, float taMax, float tbMax); 

	//! Test collision of two shapes 
	int TestShapes(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB);

	//! Get collision report
	const FNX_CollisionReport &GetReport() const;

private:
	//! Test SAT between bodies
	bool TestSAT(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB, float &colTime, FNX_Vector &colNormal, FNX_Vector &colMTD);  

	//! Test interval between bodies
	bool TestInterval(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB, const FNX_Vector &axis, FNX_Vector &MTD, bool &bValidMTD, float &tfirst, float &tlast, FNX_Vector &nfirst, FNX_Vector &nlast);

	//! Calculate contacts
	bool CalculateContacts(FNX_BaseShape *shapeA, FNX_BaseShape *shapeB, float colTime, const FNX_Vector &colNormal);

	//! Solve point-point contact
	bool SolvePPContact(const FNX_Vector &posA, const FNX_Vector &posB, const FNX_Vector &normal);

	//! Solve point-edge contact
	bool SolvePEContact(const FNX_Vector &pos, const FNX_Vector &edgeA, const FNX_Vector &edgeB, const FNX_Vector &normal, bool bSwap);  

	//! Solve edge-edge contact
	bool SolveEEContact(const FNX_Vector &edgeA1, const FNX_Vector &edgeA2, const FNX_Vector &edgeB1, const FNX_Vector &edgeB2, const FNX_Vector &normal);    
};
