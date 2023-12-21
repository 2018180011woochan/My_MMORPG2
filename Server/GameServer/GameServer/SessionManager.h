#pragma once
#include "Session.h"

class SessionManager
{
private:
	// ����, ��� ���ϰ� ����
	SessionManager() {}
	SessionManager(const SessionManager& ref) {}
	SessionManager& operator=(const SessionManager& ref) {}
	~SessionManager() {}

public:
	// �̱��� ����
	static SessionManager& getInstance() {
		static SessionManager s;
		return s;
	}

public:
	array<Session, MAX_USER> clients;
};

