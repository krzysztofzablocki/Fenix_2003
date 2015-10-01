#pragma once
/// Rotation, saved as complex number
class FNX_Rotation {
public:
	//! Components
	float re, im;

public:
	// Constructor
	FNX_Rotation() {};
	FNX_Rotation(int) : re(1), im(0) {};  
	FNX_Rotation(float radians) : re(cos(radians)), im(sin(radians)) {};
	FNX_Rotation(float are, float aim) : re(are), im(aim) {};
	FNX_Rotation(const FNX_Rotation &other) : re(other.re), im(other.im) {};

	// Assignment
	const FNX_Rotation &operator=(const FNX_Rotation &other) { re=other.re; im=other.im; return *this; }
	const FNX_Rotation &operator*=(const FNX_Rotation &other) { Set(re*other.re-im*other.im, re*other.im+im*other.re); return *this; }
	const FNX_Rotation &operator*=(float radians) { float c=cos(radians); float s=sin(radians); Set(re*c-im*s, re*s+im*c); return *this; } 

	// Operators
	FNX_Rotation operator*(const FNX_Rotation &other) const { return FNX_Rotation(*this) *= other; }
	FNX_Rotation operator*(float radians) const { return FNX_Rotation(*this) *= radians; }
	FNX_Vector operator*(const FNX_Vector &src) const { return FNX_Vector(src.x*re - src.y*im, src.x*im + src.y*re); }
	FNX_Rotation operator-() const { return FNX_Rotation(re, -im); }

	// Functions
	void Identity() { re = 1.0f; im = 0.0f; }
	float Radians() const { return atan2(im, re); }
	FNX_Vector AxisX() const { return FNX_Vector(re, im); }
	FNX_Vector AxisY() const { return FNX_Vector(-im, re); }
	void Set(float are, float aim) { re = are; im = aim; }
	void Set(float radians) { re = cos(radians); im = sin(radians); }  
};  