#pragma once
// Infiinity
#define FNX_Infinity (1e20f)

template<typename T>
inline void FNX_Limit(T & val, T min, T max) {
	if (val < min) val = min; else if (val > max) val = max;}


// Abs
template <typename T>
__forceinline T FNX_Abs(T x) {
	if (x < 0) return -x;
	return x;
}

// Sign
template <typename T>
__forceinline int FNX_Sign(T x) {
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

// Min
template <typename T>
__forceinline T FNX_Min(T x, T y) {
	if (x < y) return x;
	return y;
}

// Max
template <typename T>
__forceinline T FNX_Max(T x, T y) {
	if (x > y) return x;
	return y;
}
template <typename T> inline T FNX_Clamp	(T a, T min, T max) { return FNX_Max<T>(FNX_Min<T>(a, max), min); }

