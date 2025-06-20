#pragma once
#include <EngineCore/GameMode.h>
#include "RoomManager.h"
#include <EnginePlatform/EngineSound.h>

// Ό³Έν :
class APlayGameMode : public AGameMode
{
public:
	APlayGameMode();
	~APlayGameMode();

	void BeginPlay();
	void Tick(float _DeltaTime);

	void StartLevel() override;
	void EndLevel() override;

	static FVector MousePos;
	static FVector KnightPos;
	ACameraActor* Camera = nullptr;
	static bool& GetPauseRef()
	{
		return Pause;
	}

	URoomManager& GetRoomsRef()
	{
		return Rooms;
	}
	static APlayGameMode* GetPlayGameMode()
	{
		return PlayGameMode;
	}

protected:

private:
	inline static bool Pause = false;
	inline static APlayGameMode* PlayGameMode = nullptr;

	void SetCamera();
	void SetBasePoint();

	// CollisionGroup
	void CreateAndLinkCollisionGroup();
	void CheckDebugInput();
	
	void CheckInfo();

	void SetActiveRoom();

	void ShowPrompt();

	std::shared_ptr<class UTimeEventComponent> TimeEventer;

	URoomManager Rooms = URoomManager();

	std::shared_ptr<class UCollision> BasePointCollision = nullptr;

	USoundPlayer Sound;

	bool bInitSetting = false;

private:
	// delete Function
	APlayGameMode(const APlayGameMode& _Other) = delete;
	APlayGameMode(APlayGameMode&& _Other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _Other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _Other) noexcept = delete;
};

