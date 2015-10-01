#ifndef HVECTOR
#define HVECTOR

/// 2D vector class
class FNX_Vector {
 public:
  //! Coordinates
  float x,y;
  
 public:
  // Constructors
  FNX_Vector() {};
  FNX_Vector(int) : x(0), y(0) {};
  FNX_Vector(float ax, float ay) : x(ax), y(ay) {};
  FNX_Vector(const FNX_Vector &other) : x(other.x), y(other.y) {};
  
  // Assignment operators
  const FNX_Vector &operator=(const FNX_Vector &other) { x = other.x; y = other.y; return *this; }
  const FNX_Vector &operator+=(const FNX_Vector &other) { x += other.x; y += other.y; return *this; }
  const FNX_Vector &operator+=(float val) { x += val; y += val; return *this; }
  const FNX_Vector &operator-=(const FNX_Vector &other) { x -= other.x; y -= other.y; return *this; }
  const FNX_Vector &operator-=(float val) { x -= val; y -= val; return *this; }
  const FNX_Vector &operator*=(const FNX_Vector &other) { x *= other.x; y *= other.y; return *this; }
  const FNX_Vector &operator*=(float val) { x *= val; y *= val; return *this; }
  const FNX_Vector &operator/=(const FNX_Vector &other) { x /= other.x; y /= other.y; return *this; }
  const FNX_Vector &operator/=(float val) { val = 1.0f / val; x *= val; y *= val; return *this; }
  
  // Operators
  FNX_Vector operator+(const FNX_Vector &other) const { return FNX_Vector(x+other.x, y+other.y); }
  FNX_Vector operator+(float val) const { return FNX_Vector(x+val, y+val); }
  FNX_Vector operator-(const FNX_Vector &other) const { return FNX_Vector(x-other.x, y-other.y); }
  FNX_Vector operator-(float val) const { return FNX_Vector(x-val, y-val); }
  FNX_Vector operator*(const FNX_Vector &other) const { return FNX_Vector(x*other.x, y*other.y); }
  FNX_Vector operator*(float val) const { return FNX_Vector(x*val, y*val); }
  FNX_Vector operator/(const FNX_Vector &other) const { return FNX_Vector(x/other.x, y/other.y); }
  FNX_Vector operator/(float val) const { val = 1.0f / val; return FNX_Vector(x*val, y*val); }
  
  // Other operators
  float operator|(const FNX_Vector &other) const { return x*other.x + y*other.y; }
  FNX_Vector operator~() const { return FNX_Vector(-y, x); }  
  FNX_Vector operator-() const { return FNX_Vector(-x, -y); }  
  float &operator[](int i) { return ((float*)&x)[i]; }
  const float &operator[](int i) const { return ((const float*)&x)[i]; }
  
  // Pointer operators
  float *operator*() { return &x; }
  const float *operator*() const { return &x; }
  
  // Normalization
  float Length() const { return sqrt(x*x + y*y); }
  float SquareLength() const { return x*x + y*y; }
  float Distance(const FNX_Vector &other) const { return sqrt((x-other.x)*(x-other.x) + (y-other.y)*(y-other.y)); }
  float SquareDistance(const FNX_Vector &other) const { return (x-other.x)*(x-other.x) + (y-other.y)*(y-other.y); }
  float Normalize() { float len = Length(); if (len) { float invlen = 1.0f / len; x*=invlen; y*=invlen; } return len; }
  
  // Utility functions  
  void Zero() { x=y=0.0f; }
  void Set(float ax, float ay) { x=ax; y=ay; }
  void Axis(float radians) { x=cos(radians); y=sin(radians); }  
  void Random() { x=-1.0f + 2.0f*(rand()/(float)RAND_MAX); y=-1.0f + 2.0f*(rand()/(float)RAND_MAX); };
  FNX_Vector Min(const FNX_Vector &other) { return FNX_Vector(FNX_Min(x, other.x), FNX_Min(y, other.y)); }
  FNX_Vector Max(const FNX_Vector &other) { return FNX_Vector(FNX_Max(x, other.x), FNX_Max(y, other.y)); }
};

#endif
