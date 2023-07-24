#include "pch.h"
#include <iostream>
//#include "CoreMacro.h"

class TestLock
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;
	}

	void TestPush()
	{
		WRITE_LOCK;
	}

	void TestPop()
	{
		WRITE_LOCK;
	}

private:
	queue<int32> _queue;
};

int main()
{
	
}
