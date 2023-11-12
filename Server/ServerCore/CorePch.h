#pragma once

// 실질적으로 게임서버나 클라이언트에서 참조하길 바라는 내용들

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <thread>

#include <iostream>
#include <Windows.h>

using namespace std;

#include <Winsock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "Lock.h"

template<typename Type, typename... Args>
shared_ptr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ new Type(forward<Args>(args)...), delete Type  };
}