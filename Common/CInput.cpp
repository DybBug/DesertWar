//*****************************************************************************************
//	CInput.cpp : 입력 관련 소스 파일.
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
//	CInput 클래스.
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
	// 키보드 객체 생성.
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL))){
		return FALSE;
	}
	
	// 데이터 포멧 설정.
	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard))){
		return FALSE;
	}

	// 협력 레벨 설정.
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
	// 마우스 객체 생성.
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL))){
		return FALSE;
	}

	// 데이터 포멧 설정.
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse))){
		return FALSE;
	}

	// 협력 레벨 설정.
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
// 키 상태 변수의 '비트' 를 조정합니다.
//
// 최하위 비트 1 : 이번에 눌렸음.          : 0x01
// 최하위 비트 0 : 이번에 눌리지 않았음.   : 0x00
//
// 최상위 비트 1 : 이전에도 눌렸음.          : 0x80
// 최상위 비트 0 : 이전에도 눌리지 않았음.   : 0x00
//
//
// * 이전 (안눌림) / 현재 (안눌림) : 0x00
// * 이전 (안눌림) / 현재 (눌림 )  : 0x01  ( KeyDown)
// * 이전 (눌림)   / 현재 (안눌림) : 0x80  ( KeyUp )
// * 이전 (눌림)   / 현재 (눌림)   : 0x81  ( 계속 KeyDown)
void CInput::_CheckKeyBoardUpnDown()
{
	for (int i = 0; i < 256; ++i){
		//----------------------------------------
		// 이번에 키가 눌렸음.
		//----------------------------------------
		if (m_NewKeyBoardState[i] & 0x80){
			// 이전에 안눌렸다면.. 최하위 비트 세팅.(처음 눌림)
			if (!(m_OldKeyBoardState[i] & 0x80)){
				m_KeyBoardState[i] = 0x01;	// 최하위 비트 세팅 : Now(0x01)
			}
			// 이전에도 키가 눌렸으면(계속 눌림)
			else if (m_OldKeyBoardState[i] & 0x80){
				m_KeyBoardState[i] = 0x81;	//	최상/하위 비트 세팅 : Now(0x81)
			}
		}
		//----------------------------------------
		// 이번에 키가 눌리지 않음.
		//----------------------------------------
		else{
			// 이전에도 눌리지 않음.
			if (!(m_OldKeyBoardState[i] & 0x80)){
				m_KeyBoardState[i] = 0x00;	// 키값 완전 클리어.
			}
			// 이전에 눌렸었으면(눌렀다 떨어짐)
			else if (m_OldKeyBoardState[i] & 0x80){
				m_KeyBoardState[i] = 0x80;	// 최상위 비트만 남김.
			}
		}
	}

	// 현재값 없데이트.
	memcpy(m_OldKeyBoardState, m_NewKeyBoardState, sizeof(BYTE) * 256);
}




//////////////////////////////////////////////////////////////////////////////////////////
// _CheckMouseKeyUpnDown()
// : 마우스 키 Up and Donw 검사 함수.
void CInput::_CheckMouseKeyUpnDown()
{
	for (int i = 0; i < 256; ++i){
		m_NewMouseKeyState[i] = m_MouseState.rgbButtons[i];
		//---------------------------------------------
		// 이번에 키가 눌렸음.
		//---------------------------------------------
		if (m_NewMouseKeyState[i] & 0x80){
			// 이전에 안눌렸다면.. 최하위 비트 셋팅.(처음 눌림)
			if (!(m_OldMouseKeyState[i] & 0x80)){
				m_MouseKeyState[i] = 0x01;	// 최하위 비트 셋팅 : Now(0x01)
			}
			// 이전에도 키가 눌렸으면..(계속 눌림)
			else if (m_OldMouseKeyState[i] & 0x80){
				m_MouseKeyState[i] = 0x81;	// 최 상/하위 비트 셋팅. : Now(0x81)
			}
		}
		//---------------------------------------------
		// 이번에 키가 눌리지 않음
		//---------------------------------------------
		else{
			// 이전에도 눌리지 않았으면
			if (!(m_OldMouseKeyState[i] & 0x80)){
				m_MouseKeyState[i] = 0x00;	// 키값 완전 클리어.
			}
			// 이전에도 눌렸으면..(눌렀다 떨어짐)
			else if (m_OldMouseKeyState[i] & 0x80){
				m_MouseKeyState[i] = 0x80;	// 최 상위 비트만 남김.
			}
		}
	}
	// 현재값 업데이트.
	memcpy(m_OldMouseKeyState, m_NewMouseKeyState, sizeof(BYTE) * MBT_MAX);
}



//////////////////////////////////////////////////////////////////////////////////////////
// Init()
BOOL CInput::Init()
{
	// 이전에 장치가 있다면 제거하고 다시 생성.
	Release();

	// 클라이언트 크기 얻기.
	GameEngine->Device().GetClientSize((int*)&m_nClient_Width, (int*)&m_nClient_Height);

	// 인터페이스 생성.
	if (FAILED(DirectInput8Create(g_hIns, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDirectInput, NULL))){
		return FALSE;
	};

	// 키보드 셋업.
	if (!_InitKeyBoard()){
		Release();
		return FALSE;
	}

	// 마우스 셋업.
	if (!_InitMouse()){
		Release();
		return FALSE;
	}

	// 버퍼 데이터 제거.
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
// : 입력장치 복구 : '복구' 시 꼭 한번 호출해야함.
HRESULT CInput::Restore()
{
	if (m_pKeyBoard){
		// 입력 '요구' 하기.
		HRESULT res = m_pKeyBoard->Acquire();
		while (res == DIERR_INPUTLOST){
			res = m_pKeyBoard->Acquire();
		}
	}

	if (m_pMouse){
		// 입력 '요구' 하기.
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
// : 키보드 & 마우스 입력 체크.
BOOL CInput::CheckInput()
{
	if (_ReadKeyBoard() != DI_OK) return FALSE;
	if (_ReadMouse() != DI_OK) return FALSE;

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////
// GetMouseScreenPt()
// : 마우스 좌표 얻기.
void CInput::GetMouseScreenPt(POINT* _pPos)
{
	POINT temp;

	GetCursorPos(&temp);
	ScreenToClient(g_hWnd, &temp);

	// 화면 영역에 위치하도록 보정.
	if (m_MousePos.x > (LONG)m_nClient_Width - 1) m_MousePos.x = m_nClient_Width - 1;
	if (m_MousePos.x < 0) m_MousePos.x = 0;
	if (m_MousePos.y > (LONG)m_nClient_Height - 1) m_MousePos.y = m_nClient_Height - 1;
	if (m_MousePos.y < 0) m_MousePos.y = 0;

	// 외부 변수엥 복사.
	*_pPos = m_MousePos;
}




//////////////////////////////////////////////////////////////////////////////////////////
// GetMouseRelativePt()
// : 마우스 상대 좌표 얻기.
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
//	CMouseInput 클래스.
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