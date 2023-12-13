#pragma once

class OverlappedEXP
{
public:
	WSAOVERLAPPED over;
	WSABUF wsaBuf;
	char sendBuf[BUF_SIZE];
	COMP_TYPE compType;

public:
	OverlappedEXP();
	OverlappedEXP(char* packet);
};

