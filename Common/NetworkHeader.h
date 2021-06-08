#pragma once

#pragma warning (disable:4996)
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"Mswsock")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <iostream>
#include <vector>
#include <tchar.h>
#include <d3dx11.h>
#include <D3DX10math.h>
#include <math.h>

using namespace std;

#define BULLET_MAX 200

#define MAX_BUFFER_LENGTH 4096

#define MAX_QUEUE_LENGTH	4096
#define MAX_SESSION  10

#define DEFAULT_PORT 9876
#define SERVER_IP    "127.0.0.1"



enum IO_TYPE{
	IO_ACCEPT,  
	IO_READ,	
	IO_WRITE
};

typedef struct overlapped_ex
{
	OVERLAPPED Overlapped;	// 원래 구조체
	IO_TYPE		IoType;		// 이 구조체가 하는 일
							// IO_TYPE의 종류
							// ID_READ : 읽기 작업을 할 때 사용
							// IO_WRITE : 쓰기 작업을 할때 사용
							// IO_ACCEPT : Accept를 받았을 때 사용
	VOID		*Object;	// 부모 개체 주소
}OVERLAPPED_EX;

// UIINT		4 Byte;
// D3DXVECTOR3	12 Byte;
// D3DXMATRIX	64 Byte;

enum C_Protocol {
	//Cleint->서버
	CLOADING_COMPLETE = 0,
	CENTER_WAITROOM,
	CREADER_START,
	CUSER_READY,
	CSELECT_TEAM,
	CSELECT_CHARACTER,
	CCOMBATPLANE_POS,				// ClientID(UINT) + Position(D3DXVECTOR3) + TM(D3DXMATRIX).

	CTANK_POS,						// ClientID(UINT) + Position(D3DXVECTOR3) + TM(D3DXMATRIX) + HoodTM(D3DXMATRIX).
	CTANK_BOMB_CREATE,				// ClientID(UINT) + TeamID(UINT) + GunBarrelPos(D3DXVECTOR3) + GunBarrelFront(D3DXVECTOR3).
	CTANK_SMOKEBOMB_CREATE,
	CTANK_SMOKEBOMB_TM,

	CCOMBATPLANE_MACHINGUN_CREATE,	// ClientID(UINT) + TeamID(UINT) + LMachinGunPos(D3DXVECTOR3) + LMachinGunFront(D3DXVECTOR3) + 
									// RMachinGunPos(D3DXVECTOR3) + RMachinGunFront(D3DXVECTOR3).
	CCOMBATPLANE_BOOMB_CREATE,		// ClientID(UINT) + TeamID(UINT) + Position(D3DXVECTOR3)

	CGAME_END,

	CPROTOCOL_MAX,
	
};
enum S_Protocol{
	//서버->클라
	SCONNET_COMPLETE = 0,
	SONIODISCONNECT,
	SENTER_WAITROOM,
	SROOM_LEADER,
	SLODING_START,
	SLOADING_COMPLETE,
	SREADY_USER,
	SREADY_LEADER,
	SOBJECT_INFO,

	STANK_BOMB_CREATE,				// ClientID(UINT) + Position(D3DXVECTOR3) + Front(D3DXVECTOR3).
	STANK_BOMB_TM,
	STANK_SMOKEBOMB_CREATE,
	STANK_SMOKEBOMB_TM,

	SCOMBATPLANE_MACHINGUN_CREATE,	// ClientID(UINT) + LMachinGunPos(D3DXVECTOR3) + LMachinGunFront(D3DXVECTOR3) + 
									// RMachinGunPos(D3DXVECTOR3) + RMachinGunFront(D3DXVECTOR3).
	SCOMBATPLANE_MACHINGUN_TM,		// ClientID(UINT) + ObjID(UINT) + TM(D3DXMATRIX).

	SCOMBATPLANE_BOOMB_CREATE,
	SCOMBATPLANE_BOOMB_TM,

	SCOMINFO,

	SCHARCCNT,

	STEAMCNT,

	SCOMBATPLANE_SERCHING,
	SCOMBATPLANE_TRACE,
	SCOMBATPLANE_RETRUN,
	SCOMBATPLANE_MACHINGUN_ATTACK,
	SCOMBATPLANE_COMBATBOOMB_ATTACK,

	STANK_MOVE,
	STANK_BODY_ROT,
	STANK_ATTACK,

	SOBJECT_DEATH,
	SBULLET_DEATH,

	SNEXUS_INFO,

	SPROTOCOL_MAX,	
};

enum UserStatus{
	INTRO = 0,
	WAITROOM,
	LOADING,
	GAMEING,
	GAMEEND,
};

enum{
	TANK = 1,
	COMBATPLANE = 2,
};

enum{
	RED_TEAM = 9,
	BLUE_TEAM = 0,
};

typedef struct PLAYER_INFO
{
	UINT ClientID;
	UINT Character; // 탱크 or 비행기.
	UINT Team;
	D3DXVECTOR3 Pos;
	bool Computer;

} PLAYER_INFO;