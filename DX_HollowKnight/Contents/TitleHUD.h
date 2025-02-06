#pragma once
#include <EngineCore/HUD.h>

// Ό³Έν :
class ATitleHUD : public AHUD
{
public:
	// constrcuter destructer
	ATitleHUD();
	~ATitleHUD();

	// delete Function
	ATitleHUD(const ATitleHUD& _Other) = delete;
	ATitleHUD(ATitleHUD&& _Other) noexcept = delete;
	ATitleHUD& operator=(const ATitleHUD& _Other) = delete;
	ATitleHUD& operator=(ATitleHUD&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

	void FadeIn();
	void CreateFade();
	void FadeOut();
	void FadeChange();
	std::shared_ptr<class UImageWidget> Fade;
	float4 FadeValue = FVector::ZERO;
	float4 FadeDir = FVector::UNIT; // {1, 1, 1, 1}
	UTimeEventComponent* TimeEventer = nullptr;
private:

};

