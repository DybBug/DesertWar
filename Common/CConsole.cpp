//**********************************************************************************
//	2015.01.28.
//	CConsole.cpp : 콘솔 관련 헤더 파일. 
//**********************************************************************************
#pragma warning(disable:4996)
#include "CConsole.h"

HANDLE CConsole::m_hConsole = NULL;
BOOL CConsole::m_bUse = true;
int CConsole::m_nX = 0;
int CConsole::m_nY = 0;

char CConsole::m_cMsg[2048] = "";
char CConsole::m_cMsg2[2048] = "";
int CConsole::m_nFontColor = -1;


///////////////////////////////////////////////////////////////////////////////
// Console Thread.
void __stdcall ConsoleThread(void* p)
{
	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	char buffer[1024];
	DWORD len;
	while (1){
		ReadConsole(input, buffer, 1024, &len, 0);
	}
}





///////////////////////////////////////////////////////////////////////////////
// CConsole()
CConsole::CConsole()
{

}




///////////////////////////////////////////////////////////////////////////////
// ~CConsole()
CConsole::~CConsole()
{
	
}




///////////////////////////////////////////////////////////////////////////////
// Alloc()
bool CConsole::Alloc()
{
	if (AllocConsole() != 0){
		DWORD id;
		m_hConsole = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsoleThread, 0, 0, &id);
		if (m_hConsole) return true;
	}
	return false;
}




///////////////////////////////////////////////////////////////////////////////
// Free()
bool CConsole::Free()
{
	if (m_hConsole) TerminateThread(m_hConsole, 0);

	return FreeConsole() != 0;
}




///////////////////////////////////////////////////////////////////////////////
// Write()
void CConsole::Write(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsprintf(m_cMsg, fmt, va);
	va_end(va);

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written = 0;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_nFontColor);
	WriteConsoleA(output, m_cMsg, (DWORD)strlen(m_cMsg), &written, 0);
}




///////////////////////////////////////////////////////////////////////////////
// Write2()
void CConsole::Write2(const char* fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vsprintf(m_cMsg2, fmt, va);
	va_end(va);

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written = 0;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), m_nFontColor);
	WriteConsoleA(output, m_cMsg2, (DWORD)strlen(m_cMsg2), &written, 0);
}




///////////////////////////////////////////////////////////////////////////////
// Move()
void CConsole::Move()
{
	HWND hConsole = GetConsoleWindow();
	if (hConsole == NULL) return;

	RECT rc;
	GetWindowRect(hConsole, &rc);
	MoveWindow(hConsole, m_nX, m_nY, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);

}


//****************************** End of File "CConsole.cpp" ******************************//