#include "../DesertWar_Server//ServerIocp.h"

//���� ���� ���̺� 342p

struct ST_OBB
{
	D3DXVECTOR3 vCenterPos; // ���� �߾��� ��ǥ
	D3DXVECTOR3 vAxisDir[3]; // ���ڿ� ������ �� ���� ��������
	float fAxisLen[3]; // ������ ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� ����

	int type;			// ���θ� ������ type��(=* tag)
};

// ST_OBB ����ü �ΰ��� �����͸� ���� ���ڷ� �޾�, �� OBB�� �浹 ���θ� üũ�ϴ� �Լ�
// �浹�� TRUE����, �浹���� ������ FALSE ����

BOOL CheckOBBCollision(ST_OBB* Box1, ST_OBB* Box2);
