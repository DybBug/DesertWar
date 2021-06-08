#pragma once
#include "Common.h"

namespace S_Protocol_Func{
	typedef  void(*Func)(BYTE* _pPacket, DWORD& _Length);
	extern Func g_Protocol_Func[S_Protocol::SPROTOCOL_MAX];

	void Connect_Complete_Func(BYTE* _pPacket, DWORD& _Length);
	void DisCunnect(BYTE* _pPacket, DWORD& _Length);
	void Enter_Waitroom_Func(BYTE* _pPacket, DWORD& _Length);
	void Room_Leader_Func(BYTE* _pPacket, DWORD& _Length);
	void Loading_Start_Func(BYTE* _pPacket, DWORD& _Length);
	void Loading_Complete_Func(BYTE* _pPacket, DWORD& _Length);
	void Ready_User_Func(BYTE* _pPacket, DWORD& _Length);
	void Ready_Leader_Func(BYTE* _pPacket, DWORD& _Length);
	void Object_Info_Func(BYTE* _pPacket, DWORD& _Length);
	void Tank_Bomb_Create_Func(BYTE* _pPacket, DWORD& _Length);
	void Tank_Bomb_TM_Func(BYTE* _pPacket, DWORD& _Length);
	void Tank_SmokeBomb_Create_Func(BYTE* _pPacket, DWORD& _Length);
	void Tank_SmokeBomb_TM_Func(BYTE* _pPacket, DWORD& _Length);
	void CombatPlane_Machingun_Create_Func(BYTE* _pPacket, DWORD& _Length);
	void CombatPlane_Machingun_TM_Func(BYTE* _pPacket, DWORD& _Length);
	void CombatPlane_Boomb_Create_Func(BYTE* _pPacket, DWORD& _Length);
	void CombatPlane_Boomb_TM_Func(BYTE* _pPacket, DWORD& _Length);
	void ComInfo_Func(BYTE* _pPacket, DWORD& _Length);
	void Char_Cnt_Func(BYTE* _pPacket, DWORD& _Length);
	void Team_Cnt_Func(BYTE* _pPacket, DWORD& _Length);
	void ComBatPlane_Serching_Func(BYTE* _pPacket, DWORD& _Length);
	void ComBatPlane_Trace_Func(BYTE* _pPacket, DWORD& _Length);
	void ComBatPlane_Return_Func(BYTE* _pPacket, DWORD& _Length);
	void ComBatPlane_Machingun_Attack(BYTE* _pPacket, DWORD& _Length);
	void ComBatPlane_Combatboomb_Attack(BYTE* _pPacket, DWORD& _Length);
	void Tank_Move_Func(BYTE* _pPacket, DWORD& _Length);
	void Tank_Body_Rot_Func(BYTE* _pPacket, DWORD& _Length);
	void Tank_Attack_Func(BYTE* _pPacket, DWORD& _Length);
	void Object_Death_Func(BYTE* _pPacket, DWORD& _Length);
	void Bullet_Death_Func(BYTE* _pPacket, DWORD& _Length);
	void Nexus_Info(BYTE* _pPacket, DWORD& _Length);
}