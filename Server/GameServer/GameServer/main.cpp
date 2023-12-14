#include "pch.h"

constexpr int ThreadSize = 6;

// �ӽ�
static int id = 1;

void Disconnect(int clientID)
{

}

void Dispatch()
{
	while (true) {
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

		}
	}
}

int main() 
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	// IOCP ���ø��� ��Ʈ ����
	GIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	// ���� ����
	GSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN ServerAddr;
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PORT_NUM);
	ServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	// ���� ���ε� �� ����
	bind(GSocket, reinterpret_cast<sockaddr*>(&ServerAddr), sizeof(ServerAddr));
	listen(GSocket, SOMAXCONN);

	// ������ IOCP�� ����
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