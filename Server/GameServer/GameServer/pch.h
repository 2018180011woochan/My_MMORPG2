#pragma once
#include <iostream>

#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;

#include <thread>
#include <vector>
#include <mutex>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <string>
#include <atomic>

#include "protocol.h"
#include "Include.h"
#include "OverlappedEXP.h"

HANDLE GIocpHandle;
SOCKET GSocket;

// temp
constexpr int PORT_NUM = 7777;