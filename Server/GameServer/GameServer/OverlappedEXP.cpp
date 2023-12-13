#include "pch.h"
#include "OverlappedEXP.h"

OverlappedEXP::OverlappedEXP()
{
	wsaBuf.len = BUF_SIZE;
	wsaBuf.buf = sendBuf;
	compType = OP_RECV;
	ZeroMemory(&over, sizeof(over));
}

OverlappedEXP::OverlappedEXP(char* packet)
{
	// 0번에 패킷 길이 저장
	wsaBuf.len = packet[0];
	wsaBuf.buf = sendBuf;
	compType = OP_SEND;
	ZeroMemory(&over, sizeof(over));
	memcpy(sendBuf, packet, packet[0]);
}
