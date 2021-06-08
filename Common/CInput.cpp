//*****************************************************************************************
//	CInput.cpp : �Է� ���� �ҽ� ����.
//	
//*****************************************************************************************
#include "CGameEngine.h"
#include "CInput.h"

extern HWND			g_hWnd;
extern HINSTANCE	g_hIns;


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CInput Ŭ����.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// CInput()
CInput::CInput()
{
	m_pDirectInput = NULL;
	m_pKeyBoard = NULL;
	m_pMouse = NULL;

	ZeroMemory(m_KeyBoardState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));

	m_MousePos = POINT{ 0, 0 };

	ZeroMemory(m_OldKeyBoardState, sizeof(BYTE) * 256);
	ZeroMemory(m_NewKeyBoardState, sizeof(BYTE) * 256);

	ZeroMemory(m_MouseKeyState, sizeof(BYTE) * MBT_MAX);
	ZeroMemory(m_OldMouseKeyState, sizeof(BYTE) * MBT_MAX);
	ZeroMemory(m_NewMouseKeyState, sizeof(BYTE) * MBT_MAX);

	m_nClient_Width = 0;
	m_nClient_Height = 0;
}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CInput()
CInput::~CInput()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitKeyBoard()
BOOL CInput::_InitKeyBoard()
{
	// Ű���� ��ü ����.
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL))){
		return FALSE;
	}
	
	// ������ ���� ����.
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard))){
		return FALSE;
	}

	// ���� ���� ����.
	HRESULT res;
	res = m_pKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	if (res == DIERR_UNSUPPORTED){
		return FALSE;
	}
	if (FAILED(res)) return FALSE;

	m_pKeyBoard->Acquire();

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////
// _InitMouse()
BOOL CInput::_InitMouse()
{
	// ���콺 ��ü ����.
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL))){
		return FALSE;
	}

	// ������ ���� ����.
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse))){
		return FALSE;
	}

	// ���� ���� ����.
	HRESULT res;
	res = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (res == DIERR_UNSUPPORTED) return FALSE;
	if (FAILED(res)) return FALSE;

	m_pMouse->Acquire();

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////
// _ReadKeyBoard()
HRESULT CInput::_ReadKeyBoard()
{
	if (m_pKeyBoard == NULL) return DI_OK;

	HRESULT res;

	m_pKeyBoard->Poll();

	res = m_pKeyBoard->GetDeviceState(sizeof(BYTE) * 256, m_NewKeyBoardState);

	if (res != DI_OK) return Restore();

	_CheckKeyBoardUpnDown();


	return DI_OK;
}




//////////////////////////////////////////////////////////////////////////////////////////
// _ReadMouse()
HRESULT CInput::_ReadMouse()
{
	if (m_pMouse == NULL) return DI_OK;
	HRESULT res;

	m_pMouse->Poll();
	res = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);

	if (res != DI_OK) return Restore();

	_CheckMouseKeyUpnDown();
	return DI_OK;
}




//////////////////////////////////////////////////////////////////////////////////////////
// _CheckKeyBoardUpnDown()
//
// Ű ���� ������ '��Ʈ' �� �����մϴ�.
//
// ������ ��Ʈ 1 : �̹��� ������.          : 0x01
// ������ ��Ʈ 0 : �̹��� ������ �ʾ���.   : 0x00
//
// �ֻ��� ��Ʈ 1 : �������� ������.          : 0x80
// �ֻ��� ��Ʈ 0 : �������� ������ �ʾ���.   : 0x00
//
//
// * ���� (�ȴ���) / ���� (�ȴ���) : 0x00
// * ���� (�ȴ���) / ���� (���� )  : 0x01  ( KeyDown)
// * ���� (����)   / ���� (�ȴ���) : 0x80  ( KeyUp )
// * ���� (����)   / ���� (����)   : 0x81  ( ��� KeyDown)
void CInput::_CheckKeyBoardUpnDown()
{
	for (int i = 0; i < 256; ++i){
		//----------------------------------------
		// �̹��� Ű�� ������.
		//----------------------------------------
		if (m_NewKeyBoardState[i] & 0x80){
			// ������ �ȴ��ȴٸ�.. ������ ��Ʈ ����.(ó�� ����)
			if (!(m_OldKeyBoardState[i] & 0x80)){
				m_KeyBoardState[i] = 0x01;	// ������ ��Ʈ ���� : Now(0x01)
			}
			// �������� Ű�� ��������(��� ����)
			else if (m_OldKeyBoardState[i] & 0x80){
				m_KeyBoardState[i] = 0x81;	//	�ֻ�/���� ��Ʈ ���� : Now(0x81)
			}
		}
		//----------------------------------------
		// �̹��� Ű�� ������ ����.
		//----------------------------------------
		else{
			// �������� ������ ����.
			if (!(m_OldKeyBoardState[i] & 0x80)){
				m_KeyBoardState[i] = 0x00;	// Ű�� ���� Ŭ����.
			}
			// ������ ���Ⱦ�����(������ ������)
			else if (m_OldKeyBoardState[i] & 0x80){
				m_KeyBoardState[i] = 0x80;	// �ֻ��� ��Ʈ�� ����.
			}
		}
	}

	// ���簪 ������Ʈ.
	memcpy(m_OldKeyBoardState, m_NewKeyBoardState, sizeof(BYTE) * 256);
}




//////////////////////////////////////////////////////////////////////////////////////////
// _CheckMouseKeyUpnDown()
// : ���콺 Ű Up and Donw �˻� �Լ�.
void CInput::_CheckMouseKeyUpnDown()
{
	for (int i = 0; i < 256; ++i){
		m_NewMouseKeyState[i] = m_MouseState.rgbButtons[i];
		//---------------------------------------------
		// �̹��� Ű�� ������.
		//---------------------------------------------
		if (m_NewMouseKeyState[i] & 0x80){
			// ������ �ȴ��ȴٸ�.. ������ ��Ʈ ����.(ó�� ����)
			if (!(m_OldMouseKeyState[i] & 0x80)){
				m_MouseKeyState[i] = 0x01;	// ������ ��Ʈ ���� : Now(0x01)
			}
			// �������� Ű�� ��������..(��� ����)
			else if (m_OldMouseKeyState[i] & 0x80){
				m_MouseKeyState[i] = 0x81;	// �� ��/���� ��Ʈ ����. : Now(0x81)
			}
		}
		//---------------------------------------------
		// �̹��� Ű�� ������ ����
		//---------------------------------------------
		else{
			// �������� ������ �ʾ�����
			if (!(m_OldMouseKeyState[i] & 0x80)){
				m_MouseKeyState[i] = 0x00;	// Ű�� ���� Ŭ����.
			}
			// �������� ��������..(������ ������)
			else if (m_OldMouseKeyState[i] & 0x80){
				m_MouseKeyState[i] = 0x80;	// �� ���� ��Ʈ�� ����.
			}
		}
	}
	// ���簪 ������Ʈ.
	memcpy(m_OldMouseKeyState, m_NewMouseKeyState, sizeof(BYTE) * MBT_MAX);
}



//////////////////////////////////////////////////////////////////////////////////////////
// Init()
BOOL CInput::Init()
{
	// ������ ��ġ�� �ִٸ� �����ϰ� �ٽ� ����.
	Release();

	// Ŭ���̾�Ʈ ũ�� ���.
	GameEngine->Device().GetClientSize((int*)&m_nClient_Width, (int*)&m_nClient_Height);

	// �������̽� ����.
	if (FAILED(DirectInput8Create(g_hIns, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDirectInput, NULL))){
		return FALSE;
	};

	// Ű���� �¾�.
	if (!_InitKeyBoard()){
		Release();
		return FALSE;
	}

	// ���콺 �¾�.
	if (!_InitMouse()){
		Release();
		return FALSE;
	}

	// ���� ������ ����.
	CleanUpKeyData();

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////
// Release()
void CInput::Release()
{
	if (m_pKeyBoard)	m_pKeyBoard->Unacquire();
	if (m_pMouse)		m_pMouse->Unacquire();

	if (m_pKeyBoard)	m_pKeyBoard->Release();
	if(m_pMouse)		m_pMouse->Release();
	if(m_pDirectInput)	m_pDirectInput->Release();
}




//////////////////////////////////////////////////////////////////////////////////////////
// Restore()
// : �Է���ġ ���� : '����' �� �� �ѹ� ȣ���ؾ���.
HRESULT CInput::Restore()
{
	if (m_pKeyBoard){
		// �Է� '�䱸' �ϱ�.
		HRESULT res = m_pKeyBoard->Acquire();
		while (res == DIERR_INPUTLOST){
			res = m_pKeyBoard->Acquire();
		}
	}

	if (m_pMouse){
		// �Է� '�䱸' �ϱ�.
		HRESULT res = m_pMouse->Acquire();
		while (res == DIERR_INPUTLOST){
			res = m_pMouse->Acquire();
		}
	}

	return DI_OK;
}



//////////////////////////////////////////////////////////////////////////////////////////
// CleanUpKeyData()
void CInput::CleanUpKeyData()
{
	ZeroMemory(m_OldKeyBoardState, sizeof(BYTE) * 256);
	ZeroMemory(m_NewKeyBoardState, sizeof(BYTE) * 256);
	ZeroMemory(m_KeyBoardState, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}




//////////////////////////////////////////////////////////////////////////////////////////
// CheckInput()
// : Ű���� & ���콺 �Է� üũ.
BOOL CInput::CheckInput()
{
	if (_ReadKeyBoard() != DI_OK) return FALSE;
	if (_ReadMouse() != DI_OK) return FALSE;

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////
// GetMouseScreenPt()
// : ���콺 ��ǥ ���.
void CInput::GetMouseScreenPt(POINT* _pPos)
{
	POINT temp;

	GetCursorPos(&temp);
	ScreenToClient(g_hWnd, &temp);

	// ȭ�� ������ ��ġ�ϵ��� ����.
	if (m_MousePos.x > (LONG)m_nClient_Width - 1) m_MousePos.x = m_nClient_Width - 1;
	if (m_MousePos.x < 0) m_MousePos.x = 0;
	if (m_MousePos.y > (LONG)m_nClient_Height - 1) m_MousePos.y = m_nClient_Height - 1;
	if (m_MousePos.y < 0) m_MousePos.y = 0;

	// �ܺ� ������ ����.
	*_pPos = m_MousePos;
}




//////////////////////////////////////////////////////////////////////////////////////////
// GetMouseRelativePt()
// : ���콺 ��� ��ǥ ���.
void CInput::GetMouseRelativePt(LONG* _pX, LONG* _pY, LONG* _pZ)
{
	if (_pX) *_pX = m_MouseState.lX;
	if (_pY) *_pY = m_MouseState.lY;
	if (_pZ) *_pZ = m_MouseState.lZ;
}









//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//
//	CMouseInput Ŭ����.
//
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
// CMouseInput
CMouseInput::CMouseInput()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// ~CMouseInput()
CMouseInput::~CMouseInput()
{

}




//////////////////////////////////////////////////////////////////////////////////////////
// InitMouseScreenPt()
void CMouseInput::InitMouseScreenPt()
{
	int Width, Height;
	GameEngine->Device().GetClientSize(&Width, &Height);

	m_InitPos.x = Width / 2;
	m_InitPos.y = Height / 2;

	ClientToScreen(g_hWnd, &m_InitPos);
	SetCursorPos(m_InitPos.x, m_InitPos.y);	
}




//////////////////////////////////////////////////////////////////////////////////////////
// GetMouseScreenPt()
void CMouseInput::GetMouseScreenPt(LONG* _pX, LONG* _pY)
{
	POINT temp;
	GetCursorPos(&temp);
	ScreenToClient(g_hWnd, &temp);

	int Width, Height;
	GameEngine->Device().GetClientSize(&Width, &Height);

	if (temp.x > (LONG)Width - 1) temp.x = Width - 1;
	if (temp.x < 0) temp.x = 0;
	if (temp.y >(LONG)Height - 1) temp.y = Height - 1;
	if (temp.y < 0) temp.y = 0;
	
	*_pX = temp.x;
	*_pY = temp.y;
}




//////////////////////////////////////////////////////////////////////////////////////////
// GetMouseRelativePt()
void CMouseInput::GetMouseRelativePt(LONG* _pX, LONG* _pY)
{
	POINT temp;
	GetMouseScreenPt(&temp.x, &temp.y);

	*_pX = temp.x - m_PrevPos.x;
	*_pY = -(temp.y - m_PrevPos.y);

	temp = m_InitPos;
	ScreenToClient(g_hWnd, &temp);

	m_PrevPos.x = temp.x;
	m_PrevPos.y = temp.y;

	
	ClientToScreen(g_hWnd, &temp);
	SetCursorPos(temp.x, temp.y);
}





//****************************** End of File "CInput.cpp" ******************************//