#pragma once

/// Coordinate frame
class FNX_Coords {
 public:
  //! Rotation
  FNX_Rotation R;
  
  //! Translation
  FNX_Vector T;
  
 public:
  // Constructors
  FNX_Coords() : R(), T() {};
  FNX_Coords(int) : R(0) , T(0) {};
  FNX_Coords(const FNX_Rotation &aR, const FNX_Vector &aT) : R(aR), T(aT) {};
  FNX_Coords(const FNX_Coords &other) : R(other.R), T(other.T) {};
  
  // Assignment
  const FNX_Coords &operator=(const FNX_Coords &other) { R=other.R; T=other.T; return *this; }
  const FNX_Coords &operator*=(const FNX_Coords &other) { T = other.R * T + other.T; R *= other.R; return *this; }
  
  // Operators
  FNX_Coords operator*(const FNX_Coords &other) const { return FNX_Coords(R * other.R, R * other.T + T); }  
  FNX_Coords operator-() const { FNX_Rotation inv = -R; return FNX_Coords(inv, -(inv*T)); }
  FNX_Rotation operator*(const FNX_Rotation &rot) const { return R * rot; }
  FNX_Vector operator*(const FNX_Vector &vec) const { return R * vec + T; }
  FNX_Vector operator%(const FNX_Vector &vec) const { return R * vec; }  
  
  // Functions
  void Identity() { R.Identity(); T.Zero(); }
  void Set(const FNX_Rotation &rot, const FNX_Vector &trans) { R=rot; T=trans; }
};

