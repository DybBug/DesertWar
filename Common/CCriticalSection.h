#pragma once
#include <Windows.h>

class CCriticalSection
{
private:
	CRITICAL_SECTION m_Cs; // CRITICAL_SECTION °´Ã¼
public:
	CCriticalSection()
	{
		InitializeCriticalSection(&m_Cs);      
	}

	~CCriticalSection()
	{
		DeleteCriticalSection(&m_Cs);
	}

	void Enter()
	{
		EnterCriticalSection(&m_Cs);
	}

	void Leave()
	{
		LeaveCriticalSection(&m_Cs);
	}
};