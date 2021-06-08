//*************************************************************************************
//	CInput.h : 입력 관련 헤더 파일.
//	
//*************************************************************************************
#pragma once

#include "dinput.h"
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define SAMPLE_BUFFER_SIZE 32


static void MousePos(int x, int y){};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CInput 클래스. : DirectInput관련 클래스
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class CInput
{
public :
	enum INPUT_MOUSEBTNTYPE{
		MBT_LEFT,
		MBT_RIGHT,
		MBT_MIDDLE,

		MBT_MAX
	};
public :
	CInput();
	~CInput();

protected :
	LPDIRECTINPUT8			m_pDirectInput;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	BYTE			m_KeyBoardState[256];	// 키보드 상태값 저장 변수 배열.
	DIMOUSESTATE	m_MouseState;			// 마우스 상태.

	POINT m_MousePos;	// 마우스 현재 위치.

	// 키보드 Up and Down 체크 변수.
	BYTE m_OldKeyBoardState[256];
	BYTE m_NewKeyBoardState[256];

	// 마우스 버튼 상태 체크 변수.
	BYTE m_MouseKeyState[MBT_MAX];
	BYTE m_OldMouseKeyState[MBT_MAX];
	BYTE m_NewMouseKeyState[MBT_MAX];

	// 클라이언트 크기.(마우스 좌표시 필요.)
	UINT m_nClient_Width;
	UINT m_nClient_Height;

protected :
	BOOL _InitKeyBoard();
	BOOL _InitMouse();

	HRESULT _ReadKeyBoard();
	HRESULT _ReadMouse();

	void _CheckKeyBoardUpnDown(); // 키보드 Up and Down 검사.
	void _CheckMouseKeyUpnDown(); // 마우스 Up and Down 검사.

public :
	BOOL Init();
	HRESULT Restore();
	void Release();

	void CleanUpKeyData();

	BOOL  CheckInput();

	// 마우스의 화면 좌표 얻기.
	void GetMouseScreenPt(POINT* _pPos);

	// 마우스의 좌표 변화량 얻기.
	void GetMouseRelativePt(LONG* _pX, LONG* _pY, LONG* _pZ);
public :
	BOOL IsKeyDown(int key) { return (m_KeyBoardState[key] & 0x01); }
	BOOL IsKeyUp(int key) { return (m_KeyBoardState[key] == 0x80); }

	BOOL IsLButtonDn() { return (m_MouseKeyState[MBT_LEFT] & 0x01); }
	BOOL IsRButtonDn() { return (m_MouseKeyState[MBT_RIGHT] & 0x01); }
	BOOL IsMButtonDn() { return (m_MouseKeyState[MBT_MIDDLE] & 0x01); }

	BOOL IsLButtonUp() { return (m_MouseKeyState[MBT_LEFT] == 0x80);	}
	BOOL IsRButtonUp() { return (m_MouseKeyState[MBT_RIGHT] == 0x80); }
	BOOL IsMButtonUp() { return (m_MouseKeyState[MBT_MIDDLE] == 0x80); }

	
};






//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CMouseInput : 마우스 관련 클래스.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class CMouseInput
{
public :
	CMouseInput();
	~CMouseInput();
protected :
	POINT m_PrevPos;
	POINT m_InitPos;
public :
	void InitMouseScreenPt();	// 초기 마우스 포인터 설정.
	void GetMouseScreenPt(LONG* _pX, LONG* _pY);
	void GetMouseRelativePt(LONG* _pX, LONG* _pY);

	void SetInitPos(POINT _Pos){ m_InitPos = _Pos; }
};

//****************************** End of File "CInput.h" ******************************//