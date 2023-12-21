#pragma once

class Session
{
	OverlappedEXP recvOver;

public:
	Session()
	{

	}

	~Session()
	{

	}

public:
	void DoRecv();
	void DoSend(void* packet);

public:
	SOCKET socket;
};

