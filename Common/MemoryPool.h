///////////////////////////////////////////////////////////////////////////////////////////////
//
// 2015.01.05
// �� Ŭ������ �¶��� ���Ӽ������α׷��� (���� : �ѵ��� , ������ȭ��) �� �����Ͽ� �ۼ��Ͽ����ϴ�.
//
// MemoryPool.h
// MemoryPool�� ���� ���α׷����� ����ϴ� �޸𸮸� �̸� �Ҵ��� ���� ����ڰ� 
// new �����ڸ� ���� ��ü�� ���� �� �̸� �Ҵ�� �޸𸮸� �������ִ� �����Դϴ�.
//
///////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool : public CMultiThreadSync<T>
{
public:
	static VOID* operator new(std::size_t allocLength)
		// new�� ���� ��� ȣ��˴ϴ�.
	{
		CThreadSync Sync;

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// ���� �� �̻� �Ҵ��� �� �ִ� ������ ���� ��� ���� �Ҵ��մϴ�.
		if (!mFreePointer)
			allocBlock();

		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		// mFreePointer���� �����ϴ� ��� �տ� 4����Ʈ�� �ִ� �ּҰ� ���ϴ�.

		return ReturnPointer;
	}

		static VOID	operator delete(VOID* deletePointer)	// delete�� ������
	{
		CThreadSync Sync;

		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		// delete�� ����� NEXT�� ���� mFreePointer�� �ּҸ� �־��ݴϴ�.
		mFreePointer = static_cast<UCHAR*>(deletePointer);
		// mFreePointer�� ������Ʈ�մϴ�.
	}

private:
	static VOID	allocBlock()
	{
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		// mFreepoi nterOIl ����� ũ���� �޸𸮸� �Ҵ��մϴ�.

		UCHAR **Current = reinterpret_cast<UCHAR **>(mFreePointer);
		// �Ҵ��� �޸��� ù ����� �����͸� Current�� �ֽ��ϴ�.
		UCHAR *Next = mFreePointer;	// �Ҵ�� �޸� ù ����Դϴ�.

		for (INT i = 0; i<ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// ���� ����� ����մϴ�.
			*Current = Next;		// �Ҵ�� �޸� �� 4����Ʈ�� ���� ����� �ּҸ� �ֽ��ϴ�.
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;				// �������� ��� �տ� 4����Ʈ�� NULL�Դϴ�.
	}

private:
	static UCHAR	*mFreePointer;

protected:
	~CMemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;
