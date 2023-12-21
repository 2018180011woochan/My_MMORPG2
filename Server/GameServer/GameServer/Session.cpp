#include "pch.h"
#include "Session.h"
#include "OverlappedEXP.h"

void Session::DoRecv()
{
	DWORD recvFlag = 0;
	memset(&recvOver.over, 0, sizeof(recvOver.over));
	WSARecv(socket, &recvOver.wsaBuf, 1, 0, &recvFlag, &recvOver.over, 0);
}

void Session::DoSend(void* packet)
{
	char* cPacket = reinterpret_cast<char*>(packet);
	OverlappedEXP* sdata = new OverlappedEXP(cPacket);
	WSASend(socket, &sdata->wsaBuf, 1, 0, 0, &sdata->over, 0);
}
