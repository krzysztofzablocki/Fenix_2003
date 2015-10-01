#pragma once
#include <windows.h>

class FNX_Timer
{
private:
	LARGE_INTEGER m_dwStartTime;
	LARGE_INTEGER m_dwStartTimer;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER s_lastTime;
	LARGE_INTEGER fps_lastTime;
	LARGE_INTEGER fps_currentTime;
public:
	LARGE_INTEGER m_dwTicksPerSecond;
	FNX_Timer();
	float GetFPS();
	float GetElapsedSeconds();
	float GetTime();
};