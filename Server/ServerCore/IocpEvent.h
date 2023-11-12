#pragma once
#include "IocpCore.h"
#include "Session.h"
//class Session;
//class IocpObject;

enum class EventType : uint8
{
	Connect,
	Accept,
	Recv,
	Send
};

// OFFSET 0번엔 OVERLAPPED 메모리가 들어가 있으니
// IocpEvent의 포인터로 접근 가능
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void Init();

public:
	EventType eventType;
	shared_ptr<IocpObject> owner;
};

// ConnectEvent
class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) { }

};

// AcceptEvent
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) { }

public:
	// 디스패치할때 이벤트 정보에서 물고 있는 세션의 정보를 알아야 하니까 따로 저장
	shared_ptr<class Session> session;
};

// RecvEvent
class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) { }

};

// SendEvent
class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) { }

};