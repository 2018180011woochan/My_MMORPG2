#pragma once

// IocpObject

class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	// 세션이나 소켓을 새로 만들어주면 iocp쪽에다 등록하는 함수
	bool Register(shared_ptr<IocpObject> iocpObject);

	// worker스레드들이 iocp에 일감이 없나 찾는 함수
	bool Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;
};
