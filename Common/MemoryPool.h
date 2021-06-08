///////////////////////////////////////////////////////////////////////////////////////////////
//
// 2015.01.05
// 본 클래스는 온라인 게임서버프로그래밍 (저자 : 한동훈 , 정보문화사) 를 참고하여 작성하였습니다.
//
// MemoryPool.h
// MemoryPool은 응용 프로그램에서 사용하는 메모리를 미리 할당해 놓고 사용자가 
// new 연산자를 사용뼈 개체를 만들 때 미리 할당된 메모리를 제공해주는 형태입니다.
//
///////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool : public CMultiThreadSync<T>
{
public:
	static VOID* operator new(std::size_t allocLength)
		// new를 했을 경우 호출됩니다.
	{
		CThreadSync Sync;

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		// 만약 더 이상 할당할 수 있는 공간이 없을 경우 새로 할당합니다.
		if (!mFreePointer)
			allocBlock();

		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		// mFreePointer에는 리턴하는 블록 앞에 4바이트로 있던 주소가 들어갑니다.

		return ReturnPointer;
	}

		static VOID	operator delete(VOID* deletePointer)	// delete를 했을때
	{
		CThreadSync Sync;

		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		// delete된 블록의 NEXT에 현재 mFreePointer의 주소를 넣어줍니다.
		mFreePointer = static_cast<UCHAR*>(deletePointer);
		// mFreePointer를 업데이트합니다.
	}

private:
	static VOID	allocBlock()
	{
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		// mFreepoi nterOIl 사용할 크기의 메모리를 할당합니다.

		UCHAR **Current = reinterpret_cast<UCHAR **>(mFreePointer);
		// 할당한 메모리의 첫 블록의 포인터를 Current에 넣습니다.
		UCHAR *Next = mFreePointer;	// 할당된 메모리 첫 블록입니다.

		for (INT i = 0; i<ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// 다음 블록을 계산합니다.
			*Current = Next;		// 할당된 메모리 앞 4바이트에 다음 블록의 주소를 넣습니다.
			Current = reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;				// 마지막일 경우 앞에 4바이트는 NULL입니다.
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
