//**********************************************************************************
//	2015.01.02.
//	CTimer.h : ���̴� ���� ��� ����.
//**********************************************************************************
#pragma once
#include "Common.h"



class CTimer
{
public:
	CTimer();
	virtual ~CTimer();

private :
	double m_dSecondsPerCount;
	double m_dDeltaTime;
	
	__int64 m_dBaseTime;
	__int64 m_dPausedTime;
	__int64 m_dStopTime;
	__int64 m_dPrevTime;
	__int64 m_dCurrTime;

	bool m_bStopped;

public :
	void Reset();
	void Start();
	void Stop();
	void Tick();

	float GetDeltaTime() const { return (float)m_dDeltaTime; }
	float GetTotalTime();
};






//****************************** End of File "CTimer.h" ******************************//