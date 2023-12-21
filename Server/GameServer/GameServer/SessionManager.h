#pragma once
#include "Session.h"
//class Session;

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

private:
	array<Session, MAX_USER> clients;
};

