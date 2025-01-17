#pragma once
#include <thread>
#include <functional>

// 설명 : 언리얼 방식은 아닙니다.
// 언리얼에서 네이티브 쓰레들르 사용하려면 Run 뭐시기 였어요 녀석을 사용해야 한다.
class UEngineThread
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineThread();
	ENGINEAPI virtual ~UEngineThread();

	// delete Function
	UEngineThread(const UEngineThread& _Other) = delete;
	UEngineThread(UEngineThread&& _Other) noexcept = delete;
	UEngineThread& operator=(const UEngineThread& _Other) = delete;
	UEngineThread& operator=(UEngineThread&& _Other) noexcept = delete;

	ENGINEAPI bool Start(std::string _Name, std::function<void()> _Function);
	ENGINEAPI void Join();

protected:

private:
	std::string Name;
	std::thread ThreadInst;
	std::function<void()> ThreadFunction;

	static void ThreadBaseFunction(UEngineThread* _Thread);
};

