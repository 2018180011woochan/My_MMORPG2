#pragma once
#include "Types.h"

// RW SpinLock

// W -> R (0)
// R -> W (X)
class Lock
{
public:
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,		// 스핀락에서 최대로 기다릴 틱
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,	// 상위 16비트를 1로 세팅
		READ_COUNT_MASK = 0x0000'FFFF,		// 하위 16비트를 1로 세팅
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

// LockGuard
class ReadLockGuard
{
public:
	ReadLockGuard();
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard()  { _lock.ReadUnlock(); }

private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard();
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnlock(); }

private:
	Lock& _lock;
};

