#include "PreCompile.h"
#include "PlayHUD.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>
#include <EngineCore/TimeEventComponent.h>

APlayHUD::APlayHUD()
{
	ScreenSize = UEngineCore::GetScreenScale();
	HalfSize = ScreenSize * 0.5f;
	Knight = AKnight::GetPawn();

	TimeEventor = CreateDefaultSubObject<UTimeEventComponent>().get();
}

APlayHUD::~APlayHUD()
{
}

void APlayHUD::BeginPlay()
{
	AHUD::BeginPlay();

	CreateSkillGaugeFrame();
	CreateHPFrame();
	CreateHpUI();
	CreateGeo();
	CreateGeoCount();

	//CreateFade();
}

void APlayHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
	SetHpUI(); // 실시간 HP 개수 반영
	if (UEngineInput::IsDown('F'))
	{
		FadeOut();
	}
}

void APlayHUD::CreateSkillGaugeFrame()
{
	std::shared_ptr<UImageWidget> SkillGaugeFrame = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "SkillGaugeFrame");

	SkillGaugeFrame->SetWorldLocation({ -ScreenSize.X * SkillGaugeFramePosX,  ScreenSize.Y * SkillGaugeFramePosY });
	SkillGaugeFrame->SetTexture("018-05-118.png", true, 0.9f);
	SkillGaugeFrame->SetDownEvent([]()
		{
			UEngineDebug::OutPutString("Click");
		});
}

void APlayHUD::CreateHPFrame()
{
	int MaxHp = Knight->GetStatRef().GetMaxHp();
	for (int i = 0; i < MaxHp; i++)
	{
		std::shared_ptr<UImageWidget> HpFrame = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "HpFrame");

		HpFrame->SetWorldLocation({ - ScreenSize.X *(HpFramePosX - (HpFramePosXGap * i)),  ScreenSize.Y * HpFramePosY });
		HpFrame->SetTexture("004-00-003.png", true, 0.7f);
	}
}

void APlayHUD::CreateHpUI()
{
	int Hp = Knight->GetStatRef().GetHp();
	PrevHp = Hp - 1;
	Hps.reserve(Hp);

	for (int i = 0; i < Hp; i++)
	{
		std::shared_ptr<UImageWidget> HpUI = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK) + 10, "HpUI");
		std::string HealthRefill = "HealthRefill";
		std::string HealthIdle = "HealthIdle";
		std::string HealthAppear = "HealthAppear";
		std::string HealthBreak = "HealthBreak";

		HpUI->CreateAnimation(HealthRefill, HealthRefill, 0, 6, 0.1f, false);
		HpUI->CreateAnimation(HealthIdle, HealthIdle, {0, 1, 2, 3, 4}, {3.0f, 0.1f, 0.1f, 0.1f, 0.1f}, true);
		HpUI->CreateAnimation(HealthAppear, HealthAppear, 0, 5, 0.1f, false);
		HpUI->CreateAnimation(HealthBreak, HealthBreak, 0, 6, 0.1f, false);



		HpUI->SetWorldLocation({ -ScreenSize.X * (HpFramePosX - (HpFramePosXGap * i)),  ScreenSize.Y * HpFramePosY });
		HpUI->SetAutoScale(true);
		HpUI->SetAutoScaleRatio(0.7f);
		HpUI->ChangeAnimation(HealthRefill);
		//HpUI->SetTexture("002-40-007.png", true, 0.7f);
		Hps.push_back(HpUI);
	}
	TimeEventor->AddEndEvent(1.0f, std::bind(&APlayHUD::ChangeHpUI, this));
}

void APlayHUD::ChangeHpUI()
{
	for (int i = 0; i < PrevHp; i++)
	{
		Hps[i]->ChangeAnimation("HealthIdle");
	}

	bIsHpIdle = true;
}

void APlayHUD::SetHpUI()
{
	if (false == bIsHpIdle)
	{
		return;
	}

	int Hp = Knight->GetStatRef().GetHp();
	bool HpMinus = false;
	bool HpPlus = false;

	// 조건식 보강 필요
	if (0 == Hp)
	{
		return;
	}
	if (Hp == PrevHp + 1)
	{
		return; // 체력 변화가 없으므로
	}
	else if (Hp <= PrevHp) // 체력이 감소
	{
		HpMinus = true;
	}
	else if (Hp >= PrevHpMinusOne) // 4 , 2(3)
	{
		HpPlus = true;
	}

	if (true == HpPlus) // 체력 회복
	{
		if (Hp == Hps.size() + 1)
		{
			MSGASSERT("현재 HP보다 최대 HP가 더 많을 수 없습니다.");
			return;
		}
		//      4   == 5
		PrevHpMinusOne = PrevHp - 1;
		for (PrevHp += 1; PrevHp < Hp; ++PrevHp)
		{
			Hps[PrevHp]->ChangeAnimation("HealthAppear");
		}

		TimeEventor->AddEndEvent(1.0f, std::bind(&APlayHUD::ChangeHpUI, this));
	}
	else if (true == HpMinus) // 체력 감소
	{
		PrevHpMinusOne = PrevHp - 1;
		for (PrevHp += 1; PrevHp != Hp; --PrevHp)
		{
			Hps[PrevHp - 1]->ChangeAnimation("HealthBreak");
		}
	}
}

void APlayHUD::CreateGeo()
{
	std::shared_ptr<UImageWidget> GeoUI = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "GeoUI");
	float GeoPosX = SkillGaugeFramePosX - 0.04f;
	float GeoPosY = SkillGaugeFramePosY - 0.07f;
	GeoUI->SetWorldLocation({ -ScreenSize.X * GeoPosX,  ScreenSize.Y * GeoPosY });
	GeoUI->SetTexture("012-04-088.png", true, 0.9f);
}

void APlayHUD::CreateGeoCount()
{
	GeoCount = CreateWidget<UFontWidget>(static_cast<int>(EUIOrder::BACK), "GeoCount");

	float GeoPosX = SkillGaugeFramePosX - 0.07f;
	float GeoPosY = SkillGaugeFramePosY - 0.035f;
	GeoCount->SetWorldLocation({ -ScreenSize.X * GeoPosX,  ScreenSize.Y * GeoPosY });
	GeoCount->SetFont("NotoSerifCJKsc-Regular", 40.0f, UColor::WHITE);
	GeoCount->SetText("95");
}

void APlayHUD::CreateFade()
{
	Fade = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::FADE), "Fade");
	Fade->SetTexture("Fade.bmp", true, 1.0f);
}

void APlayHUD::FadeOut()
{
	UEngineDebug::OutPutString("Fade Out~~~~~");
	//Fade->ColorData.PlusColor = FVector::UNIT;
	//FadeValue =  FVector::UNIT;
	FadeDir   = -FVector::UNIT;
	FadeDir.W   = -1.0f;
	TimeEventor->AddUpdateEvent(10.0f, std::bind(&APlayHUD::FadeChange, this));
}

void APlayHUD::FadeIn()
{
	//Fade->ColorData.PlusColor = FVector::UNIT;
	FadeValue = FVector::ZERO;
	  FadeDir = FVector::UNIT;
	TimeEventor->AddUpdateEvent(2.0f, std::bind(&APlayHUD::FadeChange, this));
}

void APlayHUD::FadeChange()
{
	UEngineDebug::OutPutString("Fade Change : " + Fade->ColorData.MulColor.ToString());

	float DeltaTime = UEngineCore::GetDeltaTime();
	float Ratio = 0.1f;
	FadeValue += FadeDir * DeltaTime * Ratio;
	FadeValue.W += FadeDir.W * DeltaTime * Ratio;

	//Fade->ColorData.PlusColor = FadeValue;
	Fade->ColorData.MulColor = FadeValue;
}

