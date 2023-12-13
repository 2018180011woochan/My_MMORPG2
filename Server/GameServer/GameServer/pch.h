#pragma once
#include <iostream>

#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;

#include "Include.h"

HANDLE GIocpHandle;
SOCKET GSocket;

// temp
constexpr int PORT_NUM = 7777;