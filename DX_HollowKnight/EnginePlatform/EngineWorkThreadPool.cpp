#include "PreCompile.h"
#include "EngineWorkThreadPool.h"

class UWork
{
public:
	std::function<void()> Function;
};

UEngineWorkThreadPool::UEngineWorkThreadPool()
{
}

UEngineWorkThreadPool::~UEngineWorkThreadPool()
{
	IsWork = false;

	while (0 < RunningCount)
	{
		PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(EThreadStatus::DESTROY), 0, nullptr);
	}
}

void UEngineWorkThreadPool::Initialize(std::string_view _ThreadName /*= "WorkThread"*/, int _Count /*= 0*/)
{
	ThreadCount = _Count;

	if (0 >= _Count)
	{
		SYSTEM_INFO Info;
		GetSystemInfo(&Info);
		ThreadCount = Info.dwNumberOfProcessors;
	}

	RunningCount = ThreadCount;

	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	if (nullptr == IOCPHandle)
	{
		MSGASSERT("IOCP 핸들 생성에 실패했습니다.");
	}

	// IOCPHandle
	Threads.resize(ThreadCount);
	for (size_t i = 0; i < ThreadCount; i++)
	{
		Threads[i] = std::make_shared<UEngineThread>();

		Threads[i]->Start(std::string(_ThreadName) + std::to_string(i), std::bind(ThreadQueueFunction, IOCPHandle, this));
	}
}

void UEngineWorkThreadPool::ThreadQueueFunction(HANDLE _IOCPHandle, UEngineWorkThreadPool* _JobQueue)
{
	unsigned long Byte = 0;
	unsigned __int64 Ptr;

	LPOVERLAPPED OverPtr = nullptr;

	while (_JobQueue->IsWork)
	{
		GetQueuedCompletionStatus(_IOCPHandle, &Byte, &Ptr, &OverPtr, INFINITE);

		if (static_cast<int>(EThreadStatus::DESTROY) == Byte)
		{
			break;
		}

		if (static_cast<int>(EThreadStatus::WORK) == Byte)
		{
			UWork* Work = reinterpret_cast<UWork*>(Ptr);
			if (nullptr != Work)
			{
				Work->Function();

				_JobQueue->PendingCount.fetch_sub(1, std::memory_order_relaxed);
			}

			delete Work;
		}
	}

	_JobQueue->RunningCount -= 1;
	_JobQueue->PendingCount.fetch_sub(1, std::memory_order_relaxed);
}

void UEngineWorkThreadPool::WorkQueue(std::function<void()> _Work)
{
	PendingCount.fetch_add(1, std::memory_order_relaxed);

	UWork* NewWork = new UWork();
	NewWork->Function = _Work;

	PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(EThreadStatus::WORK), reinterpret_cast<ULONG_PTR>(NewWork), nullptr);
}

bool UEngineWorkThreadPool::IsIdle() const
{
	return 0 == PendingCount.load(std::memory_order_relaxed);
}
