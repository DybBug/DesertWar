//**********************************************************************************
//	2015.01.02.
//	CShader.cpp : 타이머 관련 소스 파일.
//**********************************************************************************
#pragma once
#include "CTimer.h"

////////////////////////////////////////////////////////////////////////////////////
//	CTimer()
CTimer::CTimer()
	: m_dSecondsPerCount(0.0), m_dDeltaTime(-1.0), m_dBaseTime(0),
	m_dPausedTime(0), m_dPrevTime(0), m_dCurrTime(0), m_bStopped(false)
{
	__int64 dCountsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*) &dCountsPerSec);
	m_dSecondsPerCount = 1.0 / (double)dCountsPerSec;
}




////////////////////////////////////////////////////////////////////////////////////
//	~CTimer()
CTimer::~CTimer()
{
	
}




////////////////////////////////////////////////////////////////////////////////////
//	GetTotalTime()
float CTimer::GetTotalTime()
{
	if (m_bStopped)
		return (float)(((m_dStopTime - m_dPausedTime) - m_dBaseTime) * m_dSecondsPerCount);
	else
		return (float)(((m_dCurrTime - m_dPausedTime) - m_dBaseTime) * m_dSecondsPerCount);
}




////////////////////////////////////////////////////////////////////////////////////
//	Reset()
void CTimer::Reset()
{
	__int64 CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);

	m_dBaseTime = CurrTime;
	m_dPrevTime = CurrTime;
	m_dStopTime = 0.0;
	m_bStopped = false;
}




////////////////////////////////////////////////////////////////////////////////////
//	Start()
void CTimer::Start()
{
	__int64 StartTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&StartTime);

	if (m_bStopped){
		m_dPausedTime += (StartTime - m_dStopTime);

		m_dPrevTime = StartTime;
		m_dStopTime = 0;
		m_bStopped = false;
	}
}




////////////////////////////////////////////////////////////////////////////////////
//	Stop()
void CTimer::Stop()
{
	if (!m_bStopped){
		__int64 CurrTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);

		m_dStopTime = CurrTime;
		m_bStopped = true;
	}
}






////////////////////////////////////////////////////////////////////////////////////
//	Tick()

void CTimer::Tick()
{
	if (m_bStopped){
		m_dDeltaTime = 0.0;
		return;
	}
	if (m_dDeltaTime >= (1.f / FPS)) m_dDeltaTime = 0.0f;

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_dCurrTime = currTime;

	m_dDeltaTime += (m_dCurrTime - m_dPrevTime)* m_dSecondsPerCount;

	m_dPrevTime = m_dCurrTime;
	
	if (m_dDeltaTime < 0.0) m_dDeltaTime = 0.0;
}





//****************************** End of File "CTimer.cpp" ******************************//