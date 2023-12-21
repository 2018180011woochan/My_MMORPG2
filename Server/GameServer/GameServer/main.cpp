#include "pch.h"
#include "SessionManager.h"
constexpr int ThreadSize = 6;

// 임시
static int id = 1;
HANDLE GIocpHandle;
SOCKET GSocket;

void Disconnect(int clientID)
{

}

void Dispatch()
{
	while (true) {
		SessionManager& Sessions = SessionManager::getInstance();

		DWORD numBytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over = nullptr;
		bool ret = GetQueuedCompletionStatus(GIocpHandle, &numBytes, &key, &over, INFINITE);
		OverlappedEXP* exOver = reinterpret_cast<OverlappedEXP*>(over);
		int ClientID = static_cast<int>(key);

		if (false == ret) {
			if (exOver->compType == OP_ACCEPT)
				cout << "Accept Error\n";
			else {
				cout << "GQCS Error on client[" << key << "]\n";
				Disconnect(ClientID);
				if (exOver->compType == OP_SEND) 
					delete exOver;
				continue;
			}
		}

		switch (exOver->compType) {
		case OP_ACCEPT:
			SOCKET clientSocket = reinterpret_cast<SOCKET>(exOver->wsaBuf.buf);
			
			// temp
			int clientID = id;
			id++;
			Sessions.clients[clientID].socket = clientSocket;

			CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket),
				GIocpHandle, clientID, 0);
			Sessions.clients[clientID].DoRecv();
			
			clientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

			ZeroMemory(&exOver->over, sizeof(exOver->over));
			exOver->wsaBuf.buf = reinterpret_cast<char*>(clientSocket);
			int addrSize = sizeof(SOCKADDR_IN);
			AcceptEx(GSocket, clientSocket, exOver->sendBuf, 0, addrSize + 16, addrSize + 16, 0, &exOver->over);
			break;
		}
	}
}

int main() 
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	// IOCP 컴플리션 포트 생성
	GIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	// 소켓 생성
	GSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT_NUM);
	ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	// 소켓 바인드 및 리슨
	bind(GSocket, reinterpret_cast<sockaddr*>(&ServerAddr), sizeof(ServerAddr));
	listen(GSocket, SOMAXCONN);

	// 소켓을 IOCP에 연결
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(GSocket), GIocpHandle, 0, 0);

	SOCKET ClientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OverlappedEXP over;
	over.compType = OP_ACCEPT;
	over.wsaBuf.buf = reinterpret_cast<CHAR*>(ClientSocket);

	SOCKADDR_IN ClientAddr;
	int AddrSize = sizeof(ClientAddr);
	AcceptEx(GSocket, ClientSocket, over.sendBuf, 0, AddrSize + 16, AddrSize + 16, 0, &over.over);

	vector<thread> WorkerThreads;
	for (int i = 0; i < ThreadSize; ++i)
		WorkerThreads.emplace_back(Dispatch);

	for (auto& th : WorkerThreads)
		th.join();

	closesocket(GSocket);
	WSACleanup();
	return 0;
}