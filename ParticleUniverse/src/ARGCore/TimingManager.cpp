// Useful re QPC:
// http://msdn.microsoft.com/en-us/library/windows/desktop/dn553408%28v=vs.85%29.aspx

/*
* Reliably get the current time and duration of a frame
* Pause and scale gameplay time values independent of other parts of the program.
* Reset the time or feed the clock our own values
* Work with both variable and fixed frame durations
* Avoid artifacts caused by consecutive frames with very different durations (spikes etc)
* Avoid precision problems.
*/

#include "TimingManager.h"

#include <cstdio>
#include <ctime>

#include <list>

#include <allegro5/allegro.h>

#include <Windows.h>

#include "ARGUtils.h"

__int64 g_freq = 0;

TimingManager* TimingManager::s_instance = nullptr;
//TimingManager::m_accumulatedTimes

TimingManager::TimingManager() :
	m_lastTimerReading(0)
{
	s_instance = this;
//	s_accumulatedTimes = 
	QueryPerformanceFrequency((LARGE_INTEGER *)&g_freq);
	argDebugf("QueryPerformanceCounter minimum resolution: 1/%I64d Seconds", g_freq);

	QueryPerformanceCounter((LARGE_INTEGER *)&m_lastTimerReading);
}

TimingManager::~TimingManager()
{
//	float const totalTime = g_advanceCounter * GetTimeStep();
//	float const rendersPerAdvance = (float)g_frameCounter / (float)g_advanceCounter;
//	argDebugf("total time = %.1f, total renders = %d, renders per advance = %.4f", totalTime, g_frameCounter, rendersPerAdvance);
}

float TimingManager::GetDeltaTime()
{
	__int64 newTimerValue;
	__int64 delta;
	QueryPerformanceCounter((LARGE_INTEGER *)&newTimerValue);
	delta = newTimerValue - m_lastTimerReading;
	m_lastTimerReading = newTimerValue;
#if 0
	argDebugf("%llu %llu", delta, g_freq);
#endif
	return (float)delta / (float)g_freq;
}

void TimingManager::RunTest()
{
	// Test
	std::list<double> stuff;
	for (int i = 0; i < 10000; i++)
	{
#if 1
		static __int64 lastReading = 0;
		__int64 newReading;
		__int64 diff;
		QueryPerformanceCounter((LARGE_INTEGER *)&newReading);
		diff = (newReading - lastReading);
		double const fullFrameMs = diff * 1000.0 / g_freq;
#else
		DWORD lastReading = 0;
		DWORD newReading = 0;
		DWORD diff;
		newReading = timeGetTime();
		diff = (newReading - lastReading);
		double const fullFrameMs = diff;
#endif
		lastReading = newReading;
		stuff.push_back(fullFrameMs);
	}
	for (std::list<double>::const_iterator s = stuff.begin(); s != stuff.end(); s++)
	{
		argDebugf("%f", *s);
	}
}

void TimingManager::BeginProfileSection(std::string const& name)
{
	__int64 value;
	QueryPerformanceCounter((LARGE_INTEGER *)&value);
	m_profileStartTimes[name] = value;
}

double TimingManager::EndProfileSection(std::string const& name)
{
	__int64 value;
	QueryPerformanceCounter((LARGE_INTEGER *)&value);
	double result = ((double)value - (double)m_profileStartTimes[name]) / (double)g_freq;
	m_profileStartTimes.erase(name);
//	argDebugf("%s: %f", name.c_str(), result);
	return result;
}

//static
void TimingManager::BeginAccumulatedProfileSection(std::string const& name)
{
	auto& times = s_instance->GetAccumulatedTimes();
	auto& value = times[name];	// creates entry if none present
	s_instance->BeginProfileSection(name);
}

//static
void TimingManager::EndAccumulatedProfileSection(std::string const& name)
{
	auto& times = s_instance->GetAccumulatedTimes();
	auto& entry = times[name];
	entry.time += s_instance->EndProfileSection(name);
	++entry.count;
//	argDebugf("%d", entry.count);
}