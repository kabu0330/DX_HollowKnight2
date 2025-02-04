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

	void SetNextAnimationForSkillGauge();
	bool bIsNextAnimation = false;

	void NextAnimation(std::string_view _Name);
	std::string NextAnimName = "";
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

	void InitSkillGauge();
	void CreateSkillGauge();
	void UpdateSkillGauge();
	ESkillGauge SwitchToEnumSkillGauge(int _Value);
	class UImageWidget* SkillGauge = nullptr;
	int PrevKnightMp = 0;
	bool bIsPickupMp = false;

	void CreateSkillGaugeEffect();
	class UImageWidget* SkillGaugeEffect = nullptr;

	void InitHpFrame();
	std::vector<std::shared_ptr<class UImageWidget>> Hps;
	float HpFramePosX = 0.31f;
	float HpFramePosY = 0.39f;
	float HpFramePosXGap = 0.045f;
	bool bIsHpFrame = false;
	int KnightHp = 0;
	int InitKinghtHp = 0;

	void CheckKnightHp();
	int PrevKnightHp = 0;

	void ChangeHpUI();
	void UpdateHpUI();
	bool bIsHpPlus = false;
	bool bIsHpMinus = false;
	

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

