#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class APlayGameMode : public AGameMode
{
public:
	// constrcuter destructer
	APlayGameMode();
	~APlayGameMode();

	// delete Function
	APlayGameMode(const APlayGameMode& _Other) = delete;
	APlayGameMode(APlayGameMode&& _Other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _Other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

	void LevelChangeStart() override;

	static std::shared_ptr<class ACameraActor> Camera;
	static FVector MousePos;
	static FVector KnightPos;
	
	bool& GetPauseRef()
	{
		return Pause;
	}

protected:

private:
	inline static bool Pause = false;

	void SetCamera();

	// CollisionGroup
	void CreateAndLinkCollisionGroup();
	
	void CheckInfo();

};

