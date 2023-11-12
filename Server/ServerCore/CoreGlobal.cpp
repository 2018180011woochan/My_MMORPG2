#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"

ThreadManager* GThreadManager = nullptr;
IocpCore GIocpCore;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		// ���߿� �Ŵ����� ������ �߰��Ǹ� ������ �� �������
		GThreadManager = new ThreadManager();
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		SocketUtils::Clear();
	}
} GCoreGlobal;