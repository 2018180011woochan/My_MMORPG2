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

	const int32 acceptCount = 100;		// 동접
	for (int32 i = 0; i < acceptCount; ++i) {
		AcceptEvent* acceptEvent = new AcceptEvent();

		acceptEvent->owner = shared_from_this();
		// 만약 acceptEvent->owner = shared_ptr<IocpObject>(this);
		// 요런식으로 만들면 레퍼런스1짜리 새로운 포인터를 새로 만드는것임

		// 요기 세션이 텅 비었음
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);	// 예약만. 완료통지는 워커 스레드가 관찰하다가 알게될것
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
		cout << "세션 등록 실패" << endl;
		return;
	}

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
		// 세션 정보를 acceptEvent에 연동한다
		// 그래야 dispatch에서 해당 이벤트를 뽑아봤을때 어떤 세션을 넘겨줬는지 알수있다
	if (false == SocketUtils::AcceptEx(/*리슨소켓*/_socket, /*클라이언트소켓*/session->GetSocket(), session->_recvBuffer, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING) {
			// 만약 펜딩이 아니라면 accept를 걸어주는 곳이 또 없다
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
