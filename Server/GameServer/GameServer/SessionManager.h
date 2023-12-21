#pragma once
#include "Session.h"

class SessionManager
{
private:
	// 복사, 상속 못하게 막기
	SessionManager() {}
	SessionManager(const SessionManager& ref) {}
	SessionManager& operator=(const SessionManager& ref) {}
	~SessionManager() {}

public:
	// 싱글톤 생성
	static SessionManager& getInstance() {
		static SessionManager s;
		return s;
	}

public:
	array<Session, MAX_USER> clients;
};

