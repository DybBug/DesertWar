#pragma once
#include "CNetworkSession.h"
#include "CCircularQueue.h"

//�����͸��� �����ϴ� NetworkSeesion Ŭ������ ��ӹ޾� ��Ŷ ����� �߰��Ѵ�.
class CPacketSession : public CNetworkSession
{
private:
	BYTE  mPacketBuffer[MAX_BUFFER_LENGTH];
	INT     mRemainLength;
	DWORD	mCurrentPacketNumber;
	DWORD	mLastReadPacketNumber;

	CCircularQueue mWriteQueue;
public:
	CPacketSession(VOID);
	virtual ~CPacketSession(VOID);

	BOOL Begin(VOID); 
	BOOL End(VOID);   

	BOOL WriteComplete(VOID); 

	BOOL GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength);

	BOOL ReadPacketForIocp(DWORD readLength);
	BOOL ReadPacketForEventSelect(VOID);

	BOOL WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength);
};