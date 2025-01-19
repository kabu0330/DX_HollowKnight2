#pragma once
#include <EngineCore/HUD.h>

// Ό³Έν :
class APlayHUD : public AHUD
{
public:
	// constrcuter destructer
	APlayHUD();
	~APlayHUD();

	// delete Function
	APlayHUD(const APlayHUD& _Other) = delete;
	APlayHUD(APlayHUD&& _Other) noexcept = delete;
	APlayHUD& operator=(const APlayHUD& _Other) = delete;
	APlayHUD& operator=(APlayHUD&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

protected:

private:

};

