#include "PreCompile.h"
#include "TitleGameMode.h"
#include "TitleLogo.h"
#include "EnginePlatform/EngineInput.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "TitleScene.h"
#include "TitleHUD.h"

ATitleGameMode::ATitleGameMode()
{
	TitleScene = GetWorld()->SpawnActor<ATitleScene>();
	TitleScene->SetActorLocation({ 0.0f, 0.0f, 0.0f });
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({0.0f, 0.0f, -1000.0f, 1.0f});

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	AActor::BeginPlay();
	HUD = dynamic_cast<ATitleHUD*>(GetWorld()->GetHUD());

	InitBackgroundSound();
	FadeEffect();
}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	StartSound();
}

void ATitleGameMode::LevelChangeStart()
{
}

void ATitleGameMode::LevelChangeEnd()
{
	HUD->FadeIn();
	TimeEventer->AddEndEvent(3.0f, []()
		{
			UEngineSound::AllSoundStop();
			UEngineCore::OpenLevel("Play");
		});
}

void ATitleGameMode::InitBackgroundSound()
{
	TimeEventer->AddEndEvent(6.0f, [this]()
		{
			Sound = UEngineSound::Play("Title.mp3");
		});
}

void ATitleGameMode::FadeEffect()
{
	TimeEventer->AddEndEvent(1.0f, [this]()
		{
			HUD->CreditsFadeIn();
		});

	TimeEventer->AddEndEvent(4.0f, [this]()
		{
			HUD->CreditsFadeOut();
		});
	TimeEventer->AddEndEvent(6.0f, [this]()
		{
			HUD->FadeOut();
		});
}

void ATitleGameMode::StartSound()
{
	if (UEngineInput::IsDown(VK_SPACE))
	{
		Sound = UEngineSound::Play("ui_button_confirm.wav");
		LevelChangeEnd();
	}
}

