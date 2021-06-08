#pragma once
#include "CPacketSession.h"
#include "CEventSelect.h"

class CClientSession : public CEventSelect
{
private:
	CPacketSession mSession;
	CCircularQueue mReadPacketQueue;

public:
	CClientSession(VOID);
	virtual ~CClientSession(VOID);
	BOOL	BeginTcp(LPSTR remoteAddress, USHORT remotePort);
	BOOL	End(VOID);

	BOOL	WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength);

	BOOL    ReadPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength);

	BOOL	m_bLeader;
	BOOL	m_bStart;
	BOOL	m_bAllReady;

	int m_rTank;
	int m_rComb;
	int m_bTank;
	int m_bComb;

	int m_rTeam;
	int m_bTeam;

	int m_nClientID;
	int m_nClientCnt;
	int m_nClientTeam;

	DWORD m_Protocol;
	BYTE m_Packet[MAX_BUFFER_LENGTH];
	DWORD m_PacketLength;


	virtual VOID	OnIoRead(VOID);
	virtual VOID	OnIoConnected(VOID);
	virtual VOID	OnIoDisconnected(VOID);
};