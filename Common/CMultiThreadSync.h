#pragma once 
#include "CCriticalSection.h"
template <class T>
class CMultiThreadSync
{
	friend class CThreadSync;
private:
	static CCriticalSection Sync;
public:
	class CThreadSync
	{
	public:
		CThreadSync(void)
		{
			T::Sync.Enter(); // �ش� Ŭ������ ����ȭ ��ä�� Enter() ȣ��
		}
		~CThreadSync(void)
		{
			T::Sync.Leave(); // �ش� Ŭ������ ����ȭ ��ü�� Leave() ȣ��
		}
	};
};

template <class T>
CCriticalSection CMultiThreadSync<T>::Sync;