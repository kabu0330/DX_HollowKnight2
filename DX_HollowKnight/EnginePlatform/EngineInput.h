#pragma once
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>

#include <vector>
#include <functional>

enum class EKeyEvent
{
	Down,
	Press,
	Free,
	Up,
};

class UEngineInput
{
public:
	~UEngineInput();

private:
	class UEngineKey
	{
	public:
		int Key = -1;

		bool IsDown = false;
		bool IsPress = false;
		bool IsUp = false;
		bool IsFree = true;

		float PressTime = 0.0f;
		float FreeTime = 0.0f;

		std::vector<std::function<void()>> PressEvents;
		std::vector<std::function<void()>> DownEvents;
		std::vector<std::function<void()>> UpEvents;
		std::vector<std::function<void()>> FreeEvents;

		UEngineKey()
		{
		}

		UEngineKey(int _Key)
			: Key(_Key)
		{
		}

		void CheckInputEvent();

		void CheckInput(float _DeltaTime);

		void Reset()
		{
			IsDown = false;
			IsPress = false;
			IsUp = false;
			IsFree = true;
			PressTime = 0.0f;
			FreeTime = 0.0f;
		}
		
	};

	ENGINEAPI static UEngineInput& GetInst();
	ENGINEAPI static void CheckInputEvent(float _DeltaTime);

public:
	ENGINEAPI static void CheckInput(float _DeltaTime);

	ENGINEAPI static void ResetKey();

	bool IsDoubleClick(int _KeyIndex, float _Time)
	{
		if (false == Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return Keys[_KeyIndex].IsDown && Keys[_KeyIndex].FreeTime < _Time;
	}

	static bool IsDown(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsDown;
	}

	static bool IsUp(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsUp;
	}

	static bool IsPress(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsPress;
	}

	static float IsPressTime(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].PressTime;
	}


	static bool IsFree(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsFree;
	}

	void BindAction(int _KeyIndex, EKeyEvent _EventType,  std::function<void()> _Function);

protected:

private:
	std::map<int, UEngineKey> Keys;

	UEngineInput();

private:
	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;
};

