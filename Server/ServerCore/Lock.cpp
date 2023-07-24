#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	// 동일한 스레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadID = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadID == lockThreadID) {
		_writeCount++;
		return;
	}

	// 아무도 소유 및 공유하고 있지 않을때 경합해서 소유권을 얻음
	const int64 beginTick = ::GetTickCount64();

	const uint32 desired = ((LThreadID << 16) * WRITE_THREAD_MASK);
	
	while (true) {
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount) {
			// 누군가가 readlock을 잡고 있으면 writelock도 잡을 수 없다
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			cout << "Lock TimeOut" << endl;

		this_thread::yield(); // 5000번 돌았으면 잠시 쉬고 옴
	}
}

void Lock::WriteUnlock()
{
	// ReadLock을 다 풀기 전에는 writeUnlock은 불가능
	if ((_lockFlag.load() & READ_COUNT_MASK) != 0) {
		cout << "Invalid Unlock Order" << endl;
		return;
	}

	const uint32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	// 동일한 스레드가 소유하고 있다면 무조건 성공
	const uint32 lockThreadID = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadID == lockThreadID) {
		_lockFlag.fetch_add(1);
		return;
	}

	// 아무도 소유하고 있지 않을 때 경합해서 공유 카운트를 올린다.
	const int64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount) {
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			cout << "Lock TimeOut" << endl;

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	// fetch_sub은 1을 뺀 다음 그 이전값을 뱉어주는 거기 때문에
	// 이전값이 0이면 문제가 된다
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		cout << "Multiple Unlock" << endl;
}
