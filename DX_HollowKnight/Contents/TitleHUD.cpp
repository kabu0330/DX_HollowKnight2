#include "PreCompile.h"
#include "TitleHUD.h"
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>

ATitleHUD::ATitleHUD()
{
	CreateFade();
	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

ATitleHUD::~ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
{
	AHUD::BeginPlay();

}

void ATitleHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
}

void ATitleHUD::CreateFade()
{
	Fade = CreateWidget<UImageWidget>(static_cast<int>(100000000), "Fade");
	Fade->SetTexture("Fade.png", true, 1.0f);
	Fade->SetActive(false);
}

void ATitleHUD::FadeIn()
{
	Fade->SetActive(true);
	FadeValue = FVector::NONE;
	FadeDir = FVector::ZERO;
	FadeDir.W = 2.0f;
	Fade->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(0.6f, std::bind(&ATitleHUD::FadeChange, this), [this]()
		{
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			FadeOut();
		});
}

void ATitleHUD::FadeOut()
{
	Fade->SetActive(true);
	FadeValue = FVector::ZERO;
	FadeDir = -FVector::UNIT;
	FadeDir.W = -1.0f;

	// 2�ʰ� FadeChange �Լ� ȣ���ϰ�, ������ Fade Active ���� MulColor�� ���󺹱��Ѵ�.
	// UI�� �� MulColor ���� �����ϴµ� �ϴ�. -2.0f �Ѿ�� �ٸ� UI�� ��������.
	TimeEventer->AddEvent(0.6f, std::bind(&ATitleHUD::FadeChange, this), [this]()
		{
			Fade->SetActive(false);
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void ATitleHUD::FadeChange()
{
	UEngineDebug::OutPutString("Fade Change Mul : " + Fade->ColorData.MulColor.ToString());

	float DeltaTime = UEngineCore::GetDeltaTime();
	float Ratio = 1.5f;
	FVector FadeValueTest = FadeValue;
	FadeValue.W += FadeDir.W * DeltaTime * Ratio;

	Fade->ColorData.MulColor = FadeValue;

	if (0.0f >= Fade->ColorData.MulColor.W)
	{
		Fade->ColorData.MulColor.W = 0.0f;
		return;
	}
}

