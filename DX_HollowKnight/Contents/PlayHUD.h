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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

private:
	UTimeEventComponent* TimeEventor = nullptr;

	void CreateSkillGaugeFrame();
	float SkillGaugeFramePosX = 0.35f;
	float SkillGaugeFramePosY = 0.37f;

	void CreateHPFrame();
	float HpFramePosX = 0.31f;
	float HpFramePosY = 0.39f;
	float HpFramePosXGap = 0.045f;
	bool bIsHpIdle = false;
	int PrevHp = 0;
	int PrevHpMinusOne = 0;

	void CreateHpUI();
	std::vector<std::shared_ptr<class UImageWidget>> Hps;
	void ChangeHpUI();
	void SetHpUI();

	void CreateGeo();
	void CreateGeoCount();
	std::shared_ptr<class UFontWidget> GeoCount = nullptr;


	FVector ScreenSize = FVector::ZERO;
	FVector HalfSize = FVector::ZERO;
	class AKnight* Knight = nullptr;
};

