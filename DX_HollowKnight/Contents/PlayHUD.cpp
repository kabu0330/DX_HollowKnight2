#include "PreCompile.h"
#include "PlayHUD.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>
#include <EngineCore/TimeEventComponent.h>
#include "Door.h"

APlayHUD::APlayHUD()
{
	ScreenSize = UEngineCore::GetScreenScale();
	HalfSize = ScreenSize * 0.5f;
	Knight = AKnight::GetPawn();

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

APlayHUD::~APlayHUD()
{
}

void APlayHUD::BeginPlay()
{
	AHUD::BeginPlay();

	InitSkillGaugeFrame();

	//CreateGeo();
	//CreateGeoCount();

	CreateFade();
}

void APlayHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);

	if (true == ADoor::IsDoorEnter())
	{
		FadeIn();
	}
	if (UEngineInput::IsDown('F'))
	{
		//FadeOut();
		FadeIn(); // 페이드 인에 페이드 아웃까지 같이 로직을 넣었다.
	}

	CheckKnightHp();
	InitHpFrame();

	//CreateHPFrame();
	//CreateHpUI();

	//SetHpUI(); // 실시간 HP 개수 반영
}

void APlayHUD::InitSkillGaugeFrame()
{
	SkillGaugeFrame = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "SkillGaugeFrame");
	std::string HUDFrame = "HUDFrame";
	SkillGaugeFrame->CreateAnimation(HUDFrame, HUDFrame, 0, 5, 0.2f, false);
	SkillGaugeFrame->ChangeAnimation(HUDFrame);
	SkillGaugeFrame->SetActive(false);

	// 게임 실행 이후 0.5초 뒤에 프레임 생성
	TimeEventer->AddEndEvent(0.5f, std::bind(&APlayHUD::CreateSkillGaugeFrame, this));
}

void APlayHUD::CreateSkillGaugeFrame()
{
	SkillGaugeFrame->SetActive(true);
	SkillGaugeFrame->SetWorldLocation({ -ScreenSize.X * SkillGaugeFramePosX,  ScreenSize.Y * SkillGaugeFramePosY });

	SkillGaugeFrame->SetDownEvent([]()
		{
			UEngineDebug::OutPutString("Click");
		});
}

void APlayHUD::CheckKnightHp()
{
	KnightHp = Knight->GetStatRef().GetHp();
}

void APlayHUD::InitHpFrame()
{
	if (true == SkillGaugeFrame->IsCurAnimationEnd())
	{
		bIsSkillGaugeFrame = true;
	}

	if (false == bIsSkillGaugeFrame)
	{
		return;
	}
	if (true == bIsHpFrame)
	{
		return;
	}

	Hps.reserve(KnightHp);

	for (int i = 0; i < KnightHp; i++)
	{
		// 초기 세팅
		std::shared_ptr<UImageWidget> HpUI = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK) + 10, "HpUI");
		std::string HealthRefill = "HealthRefill";
		std::string HealthIdle = "HealthIdle";
		std::string HealthBreak = "HealthBreak";

		HpUI->CreateAnimation(HealthRefill, HealthRefill, 0, 6, 0.1f, false);
		HpUI->CreateAnimation(HealthIdle, HealthIdle, { 0, 1, 2, 3, 4 }, { 3.0f, 0.1f, 0.1f, 0.1f, 0.1f }, true);
		HpUI->CreateAnimation(HealthBreak, HealthBreak, 0, 6, 0.1f, false);

		HpUI->SetWorldLocation({ -ScreenSize.X * (HpFramePosX - (HpFramePosXGap * i)),  ScreenSize.Y * HpFramePosY });
		HpUI->SetAutoScale(true);
		HpUI->SetAutoScaleRatio(0.8f);
		HpUI->ChangeAnimation(HealthRefill);
		Hps.push_back(HpUI);
	}

	bIsHpFrame = true;

	TimeEventer->AddEndEvent(1.0f, std::bind(&APlayHUD::ChangeHpUI, this));
}

void APlayHUD::ChangeHpUI()
{
	for (int i = 0; i < KnightHp; i++)
	{
		Hps[i]->ChangeAnimation("HealthIdle");
	}
}

void APlayHUD::SetHpUI()
{
	//if (false == bIsHpIdle)
	//{
	//	return;
	//}

	//int Hp = Knight->GetStatRef().GetHp();
	//bool HpMinus = false;
	//bool HpPlus = false;

	//// 조건식 보강 필요
	//if (0 == Hp)
	//{
	//	return;
	//}
	//if (Hp == PrevHp + 1)
	//{
	//	return; // 체력 변화가 없으므로
	//}
	//else if (Hp <= PrevHp) // 체력이 감소
	//{
	//	HpMinus = true;
	//}
	//else if (Hp >= PrevHpMinusOne) // 4 , 2(3)
	//{
	//	HpPlus = true;
	//}

	//if (true == HpPlus) // 체력 회복
	//{
	//	if (Hp == Hps.size() + 1)
	//	{
	//		MSGASSERT("현재 HP보다 최대 HP가 더 많을 수 없습니다.");
	//		return;
	//	}
	//	//      4   == 5
	//	PrevHpMinusOne = PrevHp - 1;
	//	for (PrevHp += 1; PrevHp < Hp; ++PrevHp)
	//	{
	//		Hps[PrevHp]->ChangeAnimation("HealthAppear");
	//	}

	//	TimeEventer->AddEndEvent(1.0f, std::bind(&APlayHUD::ChangeHpUI, this));
	//}
	//else if (true == HpMinus) // 체력 감소
	//{
	//	PrevHpMinusOne = PrevHp - 1;
	//	for (PrevHp += 1; PrevHp != Hp; --PrevHp)
	//	{
	//		Hps[PrevHp - 1]->ChangeAnimation("HealthBreak");
	//	}
	//}
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
	Fade = CreateWidget<UImageWidget>(static_cast<int>(100000000), "Fade");
	Fade->SetTexture("Fade.png", true, 1.0f);
	Fade->SetActive(false);
}

void APlayHUD::FadeOut()
{
	Fade->SetActive(true);
	FadeValue = FVector::ZERO;
	FadeDir   = -FVector::UNIT;
	FadeDir.W = -1.0f;

	// 2초간 FadeChange 함수 호출하고, 끝나면 Fade Active 끄고 MulColor도 원상복구한다.
	// UI가 다 MulColor 값을 공유하는듯 하다. -2.0f 넘어가면 다른 UI도 지워진다.
	TimeEventer->AddEvent(0.6f, std::bind(&APlayHUD::FadeChange, this), [this]()
		{
			Fade->SetActive(false);
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void APlayHUD::FadeIn()
{
	Fade->SetActive(true);
	FadeValue = FVector::NONE;
	  FadeDir = FVector::ZERO;
	FadeDir.W = 2.0f;
	Fade->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(0.6f, std::bind(&APlayHUD::FadeChange, this), [this]()
		{
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			FadeOut();
		});
}

void APlayHUD::FadeChange()
{
	UEngineDebug::OutPutString("Fade Change Mul : " + Fade->ColorData.MulColor.ToString());

	float DeltaTime = UEngineCore::GetDeltaTime();
	float Ratio = 1.5f;
	FVector FadeValueTest = FadeValue;
	FadeValue.W += FadeDir.W * DeltaTime * Ratio;

	Fade->ColorData.MulColor = FadeValue;
	//Fade->ColorData.PlusColor = FadeValue;

	if (0.0f >= Fade->ColorData.MulColor.W)
	{
		Fade->ColorData.MulColor.W = 0.0f;
		return;
	}

}

