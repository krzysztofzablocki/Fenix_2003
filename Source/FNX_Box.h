#pragma once

#undef min
#undef max

class FNX_Box {
public:
	bool isValid;           //!< Is this box valid
	FNX_Vector min, max;      //!< Extents

public:
	// Constructors
	FNX_Box() {};
	FNX_Box(int) : isValid(false), min(0), max(0) {};
	FNX_Box(const FNX_Vector &amin, const FNX_Vector &amax) : isValid(1), min(amin), max(amax) {};
	FNX_Box(const FNX_Box &other) : isValid(other.isValid), min(other.min), max(other.max) {};

	// Assignment operators
	const FNX_Box &operator=(const FNX_Box &other) { isValid = other.isValid; min = other.min; max = other.max; return *this; }
	const FNX_Box &operator+=(const FNX_Box &other) { Merge(other); return *this; }
	const FNX_Box &operator+=(const FNX_Vector &pos) { Merge(pos); return *this; }

	// Functions
	void Zero() { isValid = false; min.Zero(); max.Zero(); }
	void Set(const FNX_Box &other) { isValid = other.isValid; min = other.min; max = other.max; } 
	void Set(const FNX_Vector &amin, const FNX_Vector &amax) { isValid = true; min = amin; max = amax; }
	void Merge(const FNX_Box &other) { if (!isValid) { Set(other); } else if (other.isValid) { min = min.Min(other.min); max = max.Max(other.max); } }
	void Merge(const FNX_Vector &pos) { if (!isValid) { Set(pos,pos); } else { min = min.Min(pos); max = max.Max(pos); } }  
	void Extend(const FNX_Vector &margin) { min -= margin; max += margin; }
	void Extend(float margin) { min -= margin; max += margin; }

	// Checking
	bool Touches(const FNX_Box &other) const { return isValid && other.isValid && (max.x >= other.min.x) && (max.y >= other.min.y) && (min.x <= other.max.x) && (min.y <= other.max.y); }
	bool Touches(const FNX_Vector &point) const { return isValid && (point.x >= min.x) && (point.y >= min.y) && (point.x <= max.x) && (point.y <= max.y); }
	FNX_Vector Corner(int i) const { return FNX_Vector(i&1 ? min.x : max.x, i&2 ? min.y : max.y); }
};