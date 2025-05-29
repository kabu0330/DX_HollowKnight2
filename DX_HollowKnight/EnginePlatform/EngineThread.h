#pragma once
#include <thread>
#include <functional>

// Ό³Έν : 
class UEngineThread
{
public:
	ENGINEAPI UEngineThread();
	ENGINEAPI virtual ~UEngineThread();

	ENGINEAPI bool Start(std::string _Name, std::function<void()> _Function);
	ENGINEAPI void Join();

protected:

private:
	std::string Name;
	std::thread ThreadInst;
	std::function<void()> ThreadFunction;

	static void ThreadBaseFunction(UEngineThread* _Thread);

private:
	// delete Function
	UEngineThread(const UEngineThread& _Other) = delete;
	UEngineThread(UEngineThread&& _Other) noexcept = delete;
	UEngineThread& operator=(const UEngineThread& _Other) = delete;
	UEngineThread& operator=(UEngineThread&& _Other) noexcept = delete;
};

