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
		// 나중에 매니저가 여러개 추가되면 순서를 잘 맞춰야함
		GThreadManager = new ThreadManager();
		SocketUtils::Init();
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		SocketUtils::Clear();
	}
} GCoreGlobal;