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

	// �����̳� ������ ���� ������ָ� iocp�ʿ��� ����ϴ� �Լ�
	bool Register(shared_ptr<IocpObject> iocpObject);

	// worker��������� iocp�� �ϰ��� ���� ã�� �Լ�
	bool Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE _iocpHandle;
};
