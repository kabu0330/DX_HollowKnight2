#pragma once
#include <EngineCore/GameMode.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/TimeEventComponent.h>

// ���� :
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

protected:
	void InitBackgroundSound();
	void StartSound();

private:
	std::shared_ptr<class ATitleScene> TitleScene;
	UTimeEventComponent* TimeEventer = nullptr;
	USoundPlayer Sound;
	USoundPlayer ButtonSound;
	float Volume = 1.0f;
	bool bCanNextMode = false;
	bool bIsSpace = false;
	class ATitleHUD* HUD = nullptr;

private:
	// delete Function
	ATitleGameMode(const ATitleGameMode& _Other) = delete;
	ATitleGameMode(ATitleGameMode&& _Other) noexcept = delete;
	ATitleGameMode& operator=(const ATitleGameMode& _Other) = delete;
	ATitleGameMode& operator=(ATitleGameMode&& _Other) noexcept = delete;
};

