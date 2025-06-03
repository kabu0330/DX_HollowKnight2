#pragma once
#include <EngineCore/HUD.h>

// Ό³Έν :
class ATitleHUD : public AHUD
{
public:
	ATitleHUD();
	~ATitleHUD();

	void FadeIn();
	void FadeInAndOut();
	void FadeOut();

	void CreditsFadeIn();
	void CreditsFadeOut();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;


	void CreateFade();
	std::shared_ptr<class UImageWidget> Fade;
	float4 FadeValue = FVector::ZERO;
	float4 FadeDir = FVector::UNIT; // {1, 1, 1, 1}
	void FadeChange();

	void CreateCreditsLogo();
	void CreditsFadeChange();

	std::shared_ptr<class UImageWidget> CreditsLogo;
	float4 CreditsFadeValue = FVector::ZERO;
	float4 CreditsFadeDir = FVector::UNIT; 

	UTimeEventComponent* TimeEventer = nullptr;

private:
	// delete Function
	ATitleHUD(const ATitleHUD& _Other) = delete;
	ATitleHUD(ATitleHUD&& _Other) noexcept = delete;
	ATitleHUD& operator=(const ATitleHUD& _Other) = delete;
	ATitleHUD& operator=(ATitleHUD&& _Other) noexcept = delete;
};

