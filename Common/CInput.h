//*************************************************************************************
//	CInput.h : �Է� ���� ��� ����.
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
//	CInput Ŭ����. : DirectInput���� Ŭ����
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

	BYTE			m_KeyBoardState[256];	// Ű���� ���°� ���� ���� �迭.
	DIMOUSESTATE	m_MouseState;			// ���콺 ����.

	POINT m_MousePos;	// ���콺 ���� ��ġ.

	// Ű���� Up and Down üũ ����.
	BYTE m_OldKeyBoardState[256];
	BYTE m_NewKeyBoardState[256];

	// ���콺 ��ư ���� üũ ����.
	BYTE m_MouseKeyState[MBT_MAX];
	BYTE m_OldMouseKeyState[MBT_MAX];
	BYTE m_NewMouseKeyState[MBT_MAX];

	// Ŭ���̾�Ʈ ũ��.(���콺 ��ǥ�� �ʿ�.)
	UINT m_nClient_Width;
	UINT m_nClient_Height;

protected :
	BOOL _InitKeyBoard();
	BOOL _InitMouse();

	HRESULT _ReadKeyBoard();
	HRESULT _ReadMouse();

	void _CheckKeyBoardUpnDown(); // Ű���� Up and Down �˻�.
	void _CheckMouseKeyUpnDown(); // ���콺 Up and Down �˻�.

public :
	BOOL Init();
	HRESULT Restore();
	void Release();

	void CleanUpKeyData();

	BOOL  CheckInput();

	// ���콺�� ȭ�� ��ǥ ���.
	void GetMouseScreenPt(POINT* _pPos);

	// ���콺�� ��ǥ ��ȭ�� ���.
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
//	CMouseInput : ���콺 ���� Ŭ����.
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
	void InitMouseScreenPt();	// �ʱ� ���콺 ������ ����.
	void GetMouseScreenPt(LONG* _pX, LONG* _pY);
	void GetMouseRelativePt(LONG* _pX, LONG* _pY);

	void SetInitPos(POINT _Pos){ m_InitPos = _Pos; }
};

//****************************** End of File "CInput.h" ******************************//