#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/TimeEventComponent.h>
#include <EnginePlatform/EngineThread.h>

// Ό³Έν :
class ATitleGameMode : public AGameMode
{
public:
	ATitleGameMode();
	~ATitleGameMode();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void StartLevel() override;
	void EndLevel() override;

	void FadeEffect();

	static bool IsExecute()
	{
		return bIsExecute;
	}

	static bool IsPlayStart()
	{
		return bIsPlayStart;
	}

	bool IsResourceLoading() const
	{
		return bEndInitTask;
	}


protected:
	void InitBackgroundSound();
	void StartPlayGameMode();
	void SetupPlayGameMode();

	void StartLoadPlayLevelResource();
	void CheckPlayLoadComplete();
private:
	std::shared_ptr<class ATitleScene> TitleScene;
	UTimeEventComponent* TimeEventer = nullptr;
	USoundPlayer Sound;
	USoundPlayer ButtonSound;
	float Volume = 1.0f;
	bool bCanNextMode = false;
	bool bIsSpace = false;
	class ATitleHUD* HUD = nullptr;

	inline static bool bIsExecute = false;
	inline static bool bIsPlayStart = false;

	UEngineThread Thread;
	std::atomic<bool> bTitleLoadFinished = false;
	bool bEndInitTask = true;

	class ULevel* PlayWorld = nullptr;
	bool bIsReady = false;

private:
	// delete Function
	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;
};

// debug
extern clock_t PlayStartLoadingTime;
extern clock_t EndLoadingTime;