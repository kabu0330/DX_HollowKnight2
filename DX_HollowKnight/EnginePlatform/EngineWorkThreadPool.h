#pragma once
#include <functional>
#include "EngineThread.h"
#include <atomic>

enum class EThreadStatus
{
	WORK = -2,
	DESTROY = -1,
};

// 설명 :
class UEngineWorkThreadPool
{
public:
	ENGINEAPI UEngineWorkThreadPool();
	ENGINEAPI ~UEngineWorkThreadPool();

	ENGINEAPI void Initialize(std::string_view ThreadName = "WorkThread", int Count = 0);

	ENGINEAPI void WorkQueue(std::function<void()> _Work);

	ENGINEAPI bool IsIdle() const;


protected:

private:
	std::atomic<int> RunningCount; // 살아있는 스레드 수

	std::atomic<bool> IsWork = true;

	HANDLE IOCPHandle = nullptr;

	std::atomic<int> PendingCount{ 0 }; // 작업 스레드 수

	int ThreadCount = 0;
	std::vector<std::shared_ptr<UEngineThread>> Threads;

	static void ThreadQueueFunction(HANDLE _IOCPHandle, UEngineWorkThreadPool* _JobQueue);

private:
	// delete Function
	UEngineWorkThreadPool(const UEngineWorkThreadPool& _Other) = delete;
	UEngineWorkThreadPool(UEngineWorkThreadPool&& _Other) noexcept = delete;
	UEngineWorkThreadPool& operator=(const UEngineWorkThreadPool& _Other) = delete;
	UEngineWorkThreadPool& operator=(UEngineWorkThreadPool&& _Other) noexcept = delete;
};

