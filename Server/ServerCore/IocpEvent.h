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

// OFFSET 0���� OVERLAPPED �޸𸮰� �� ������
// IocpEvent�� �����ͷ� ���� ����
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
	// ����ġ�Ҷ� �̺�Ʈ �������� ���� �ִ� ������ ������ �˾ƾ� �ϴϱ� ���� ����
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