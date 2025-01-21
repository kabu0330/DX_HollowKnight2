#include "PreCompile.h"
#include "Fade.h"
#include <EngineCore/EngineCore.h>
#include <EngineCore/ImageWidget.h>
#include <EngineCore/TimeEventComponent.h>

AFade::AFade()
{
	SetName("Fade");

	Fade = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::FADE), "Fade").get();
	Fade->SetTexture("Fade.bmp");
	FVector ScreenSize = UEngineCore::GetScreenScale();
	Fade->SetScale3D(ScreenSize);
	TimeEventor = CreateDefaultSubObject<UTimeEventComponent>().get();


}

AFade::~AFade()
{
}

void AFade::FadeIn()
{
	FadeValue = 0.0f;
	FadeDir = 1.0f;
	//TimeEventor.PushEvent(2.0f, std::bind(&AFade::FadeChange, this), true, false);
}

void AFade::FadeOut()
{
	FadeValue = 1.0f;
	FadeDir = -1.0f;
	TimeEventor->AddUpdateEvent(1.0f, std::bind(&AFade::FadeChange, this), true);
}

void AFade::FadeChange()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	FadeValue += DeltaTime * 0.5F * FadeDir;
	//Fade->SetAlphafloat(FadeValue);
}

void AFade::BeginPlay()
{
	AHUD::BeginPlay();
}

void AFade::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
}



