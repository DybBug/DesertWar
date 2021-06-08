//**********************************************************************************
//	2015.01.28.
//	CConsole.h : 콘솔 관련 헤더 파일. 
//**********************************************************************************
#pragma once
#include "Common.h"

class CConsole
{
public :
	CConsole();
	~CConsole();

private :
	static	HANDLE m_hConsole;

	static char m_cMsg[2048];
	static char m_cMsg2[2048];
public :
	static BOOL m_bUse;
	static int m_nX;
	static int m_nY;
	static int m_nFontColor;
public :
	static bool Alloc();
	static bool Free();

	static void Write(const char* fmt, ...);
	static void Write2(const char* fmt, ...);
	static void Move();

	static HANDLE GetThread() { return m_hConsole; }

	static char* GetMsg() { return m_cMsg; }
	static char* GetMsg2() { return m_cMsg2; }
};



//****************************** End of File "CConsole.h" ******************************//