#pragma once
// define 같은것들

#define OUT

// LOCK
#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx // ##을 넣으면 컴파일러가 idx값으로 치환해줌
#define READ_LOCK				READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx // ##을 넣으면 컴파일러가 idx값으로 치환해줌
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

// Crash
#define CRASH(cause)						\
{											\
	uint32* crach = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}		

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr))							\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}											\