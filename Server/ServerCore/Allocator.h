#pragma once

// 할당 정책을 정의

// BaseAllocator

class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void* Release(void* ptr);
};

