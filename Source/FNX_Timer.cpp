#include "FNX_Timer.h"


FNX_Timer::FNX_Timer()
{
	if (!QueryPerformanceFrequency(&m_dwTicksPerSecond))
			return;
		QueryPerformanceCounter(&m_dwStartTime);
		s_lastTime = m_dwStartTime;

		fps_lastTime=m_dwStartTime;
};


float FNX_Timer::GetFPS()
{
	//static LARGE_INTEGER s_lastTime = m_dwStartTime;
		
 
		QueryPerformanceCounter(&currentTime);
		float fps = static_cast<float>(m_dwTicksPerSecond.QuadPart) 
			/ (static_cast<float>(currentTime.QuadPart) - static_cast<float>(fps_lastTime.QuadPart));
		fps_lastTime = currentTime;
		return fps;
};

float FNX_Timer::GetElapsedSeconds()
{
	QueryPerformanceCounter(&currentTime);
	float Seconds= (static_cast<float>(currentTime.QuadPart)-static_cast<float>(s_lastTime.QuadPart))/static_cast<float>(m_dwTicksPerSecond.QuadPart);
    s_lastTime = currentTime;

   return Seconds;
};

float FNX_Timer::GetTime()
{
	LARGE_INTEGER s;
	QueryPerformanceCounter(&s);

	return (static_cast<float>(s.QuadPart)/static_cast<float>(m_dwTicksPerSecond.QuadPart));
};
