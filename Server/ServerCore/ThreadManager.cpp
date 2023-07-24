#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

ThreadManager::ThreadManager()
{
	// MainTread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]() 
		{
			InitTLS();			// ÷�� ������ ��������� InitTLS ȣ��
			callback();			// callback���� �Ѱ��� �Լ� ����
			DestroyTLS();		// TLS ����
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads) {
		if (t.joinable())
			t.join();
	}
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadID = 1;
	LThreadID = SThreadID.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
