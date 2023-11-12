#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"

Listener::~Listener()
{
	SocketUtils::Close(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents) {
		delete acceptEvent;
		acceptEvent = nullptr;
	}
}

bool Listener::StartAccept(NetAddress address)
{
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	if (GIocpCore.Register(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	if (SocketUtils::Bind(_socket, address) == false)
		return false;

	if (SocketUtils::Listen(_socket) == false)
		return false;

	const int32 acceptCount = 100;		// ����
	for (int32 i = 0; i < acceptCount; ++i) {
		AcceptEvent* acceptEvent = new AcceptEvent();

		acceptEvent->owner = shared_from_this();
		// ���� acceptEvent->owner = shared_ptr<IocpObject>(this);
		// �䷱������ ����� ���۷���1¥�� ���ο� �����͸� ���� ����°���

		// ��� ������ �� �����
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);	// ���ุ. �Ϸ������� ��Ŀ �����尡 �����ϴٰ� �˰Եɰ�
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	//shared_ptr<Session> session = _service->CreateSession();
	shared_ptr<Session> session = make_shared<Session>();
	if (GIocpCore.Register(session) == false)
	{
		cout << "���� ��� ����" << endl;
		return;
	}

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
		// ���� ������ acceptEvent�� �����Ѵ�
		// �׷��� dispatch���� �ش� �̺�Ʈ�� �̾ƺ����� � ������ �Ѱ������ �˼��ִ�
	if (false == SocketUtils::AcceptEx(/*��������*/_socket, /*Ŭ���̾�Ʈ����*/session->GetSocket(), session->_recvBuffer, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			// ���� ����� �ƴ϶�� accept�� �ɾ��ִ� ���� �� ����
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = acceptEvent->session;

	if (false == SocketUtils::SetUpdataAcceptSocket(session->GetSocket(), _socket)) {
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeofSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeofSockAddr)) {
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));

	cout << "Client Conneted!" << endl;

	RegisterAccept(acceptEvent);
}
