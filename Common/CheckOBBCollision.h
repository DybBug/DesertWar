#include "../DesertWar_Server//ServerIocp.h"

//게임 물리 바이블 342p

struct ST_OBB
{
	D3DXVECTOR3 vCenterPos; // 상자 중앙의 좌표
	D3DXVECTOR3 vAxisDir[3]; // 상자에 평행한 세 축의 단위벡터
	float fAxisLen[3]; // 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응

	int type;			// 서로를 구별할 type값(=* tag)
};

// ST_OBB 구조체 두개의 포인터를 각각 인자로 받아, 두 OBB의 충돌 여부를 체크하는 함수
// 충돌시 TRUE리턴, 충돌하지 않으면 FALSE 리턴

BOOL CheckOBBCollision(ST_OBB* Box1, ST_OBB* Box2);
