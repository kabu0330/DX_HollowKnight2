#pragma once
#include <Windows.h>

// Ό³Έν :
class UEngineTimer
{
public:
	ENGINEAPI UEngineTimer();
	ENGINEAPI ~UEngineTimer();

	ENGINEAPI void TimeCheck();

	ENGINEAPI float GetDeltaTime()
	{
		// 0.0000001
		return fDeltaTime;
	}

	ENGINEAPI double GetDoubleDeltaTime()
	{
		// 0.000000000000001
		return DeltaTime;
	}

	ENGINEAPI void TimeStart();

	ENGINEAPI float End();

	ENGINEAPI double DEnd();

	ENGINEAPI void SetTimeScale(float _Scale)
	{
		TimeScale = _Scale;
	}

protected:

private:
	// LONGLONG QuadPart => __int64
	LARGE_INTEGER Count = LARGE_INTEGER(); 
	LARGE_INTEGER PrevTime = LARGE_INTEGER();
	LARGE_INTEGER CurTime = LARGE_INTEGER();

	double TimeCounter = 0.0f;
	double DeltaTime = 0.0;
	float fDeltaTime = 0.0f;
	float TimeScale = 1.0f;

private:
	// delete Function
	UEngineTimer(const UEngineTimer& _Other) = delete;
	UEngineTimer(UEngineTimer&& _Other) noexcept = delete;
	UEngineTimer& operator=(const UEngineTimer& _Other) = delete;
	UEngineTimer& operator=(UEngineTimer&& _Other) noexcept = delete;
};

