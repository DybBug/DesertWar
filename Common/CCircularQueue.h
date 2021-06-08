#pragma once
#include "NetworkHeader.h"
#include "CMultiThreadSync.h"

typedef struct _QUEUE_DATA
{
	VOID	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;



class CCircularQueue : public CMultiThreadSync < CCircularQueue >
{
private:
	QUEUE_DATA		mQueue[MAX_QUEUE_LENGTH]; // ���� ť�� ���� �����Ͱ� ����Ǵ� �޸� ����
	DWORD			mQueueHead; // HEAD ������ �ϴ� ����
	DWORD			mQueueTail; // TAIL ������ �ϴ� ����

public:
	CCircularQueue(VOID);
	~CCircularQueue(VOID);

public:
	BOOL    Begin(VOID);
	BOOL    End(VOID);

	BYTE*	Push(VOID *object, BYTE *data, DWORD dataLength);
	BYTE*	Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength);

	BOOL    Pop(VOID);
	BOOL	Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength);
};