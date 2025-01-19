#include "PreCompile.h"
#include "PlayHUD.h"
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>

APlayHUD::APlayHUD()
{
	ScreenSize = UEngineCore::GetScreenScale();
	HalfSize = ScreenSize * 0.5f;
	Knight = AKnight::GetPawn();
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

	{
		std::shared_ptr<UFontWidget> Widget = CreateWidget<UFontWidget>(1);

		Widget->SetWorldLocation({ 200, 300 });
		Widget->SetFont("TrajanPro-Regular");
		Widget->SetText("안녕하세요");
	}
}

void APlayHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
	SetHpUI(); // 실시간 HP 개수 반영
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
	int MaxHp = Knight->GetStatRef().GetMaxHP();
	for (int i = 0; i < MaxHp; i++)
	{
		std::shared_ptr<UImageWidget> HpFrame = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "HpFrame");

		HpFrame->SetWorldLocation({ - ScreenSize.X *(HpFramePosX - (HpFramePosXGap * i)),  ScreenSize.Y * HpFramePosY });
		HpFrame->SetTexture("004-00-003.png", true, 0.7f);
	}
}

void APlayHUD::CreateHpUI()
{
	int Hp = Knight->GetStatRef().GetHP();
	Hps.reserve(Hp);

	for (int i = 0; i < Hp; i++)
	{
		std::shared_ptr<UImageWidget> HpUI = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "HpUI");

		HpUI->SetWorldLocation({ -ScreenSize.X * (HpFramePosX - (HpFramePosXGap * i)),  ScreenSize.Y * HpFramePosY });
		HpUI->SetTexture("002-40-007.png", true, 0.7f);
		Hps.push_back(HpUI);
	}
}

void APlayHUD::SetHpUI()
{
	int Hp = Knight->GetStatRef().GetHP();
	int Count = 0;
	for (int i = 0; i < Hps.size(); i++)
	{
		if (true == Hps[i]->IsActive())
		{
			++Count;
		}
	}

	if (Hp == Count)
	{
		return;
	}
	else if (Hp > Count)
	{
		if (Hp == Hps.size() + 1)
		{
			MSGASSERT("현재 HP보다 최대 HP가 더 많을 수 없습니다.");
			return;
		}
		Hps[Hp - 1]->SetActive(true);
	}
	else if (Hp < Count)
	{
		Hps[Hp]->SetActive(false);
	}
}

