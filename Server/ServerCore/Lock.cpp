#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock()
{
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadID = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadID == lockThreadID) {
		_writeCount++;
		return;
	}

	// �ƹ��� ���� �� �����ϰ� ���� ������ �����ؼ� �������� ����
	const int64 beginTick = ::GetTickCount64();

	const uint32 desired = ((LThreadID << 16) * WRITE_THREAD_MASK);
	
	while (true) {
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount) {
			// �������� readlock�� ��� ������ writelock�� ���� �� ����
			uint32 expected = EMPTY_FLAG;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			cout << "Lock TimeOut" << endl;

		this_thread::yield(); // 5000�� �������� ��� ���� ��
	}
}

void Lock::WriteUnlock()
{
	// ReadLock�� �� Ǯ�� ������ writeUnlock�� �Ұ���
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
	// ������ �����尡 �����ϰ� �ִٸ� ������ ����
	const uint32 lockThreadID = (_lockFlag.load() && WRITE_THREAD_MASK) >> 16;
	if (LThreadID == lockThreadID) {
		_lockFlag.fetch_add(1);
		return;
	}

	// �ƹ��� �����ϰ� ���� ���� �� �����ؼ� ���� ī��Ʈ�� �ø���.
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
	// fetch_sub�� 1�� �� ���� �� �������� ����ִ� �ű� ������
	// �������� 0�̸� ������ �ȴ�
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		cout << "Multiple Unlock" << endl;
}