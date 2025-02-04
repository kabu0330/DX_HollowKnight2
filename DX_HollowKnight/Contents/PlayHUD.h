#pragma once
#include <EngineCore/HUD.h>
#include <vector>

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

	void FadeIn();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

private:
	FVector ScreenSize = FVector::ZERO;
	FVector HalfSize = FVector::ZERO;
	class AKnight* Knight = nullptr;
	UTimeEventComponent* TimeEventer = nullptr;

	void InitSkillGaugeFrame();
	void CreateSkillGaugeFrame();
	std::shared_ptr<class UImageWidget> SkillGaugeFrame = nullptr;
	float SkillGaugeFramePosX = 0.35f;
	float SkillGaugeFramePosY = 0.39f;
	bool bIsSkillGaugeFrame = false;

	void InitHpFrame();
	float HpFramePosX = 0.31f;
	float HpFramePosY = 0.39f;
	float HpFramePosXGap = 0.045f;
	bool bIsHpIdle = false;
	bool bIsHpFrame = false;

	void CheckKnightHp();
	int KnightHp = 0;

	std::vector<std::shared_ptr<class UImageWidget>> Hps;
	void ChangeHpUI();
	void SetHpUI();

	void CreateGeo();
	void CreateGeoCount();
	std::shared_ptr<class UFontWidget> GeoCount = nullptr;


	void CreateFade();
	void FadeOut();
	void FadeChange();
	std::shared_ptr<class UImageWidget> Fade;
	float4 FadeValue = FVector::ZERO;
	float4 FadeDir = FVector::UNIT; // {1, 1, 1, 1}
};

