#include "PreCompile.h"
#include "TitleGameMode.h"
#include "EnginePlatform/EngineInput.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "TitleScene.h"
#include "TitleHUD.h"
#include <EngineCore/EngineCore.h>
#include "ContentsResource.h"
#include "PlayGameMode.h"
#include "PlayHUD.h"

// debug
clock_t PlayStartLoadingTime = -1;
clock_t EndLoadingTime = -1;

ATitleGameMode::ATitleGameMode()
{
	Thread.Start("TitleLoading", [this]()
		{
			UContentsResource::LoadTitleResource();
			bTitleLoadFinished = true;
		});

	TimeEventer = CreateDefaultSubobject<UTimeEventComponent>().get();
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CheckPlayLoadComplete();

	StartLoadPlayLevelResource();

	StartPlayGameMode();

}

void ATitleGameMode::InitBackgroundSound()
{
	TimeEventer->AddEndEvent(5.0f, [this]()
		{
			Sound = UEngineSound::Play("Title.mp3");
			Sound.Loop(999);
		});
}

void ATitleGameMode::FadeEffect()
{
	TimeEventer->AddEndEvent(0.5f, [this]()
		{
			HUD->CreditsFadeIn();
		});

	TimeEventer->AddEndEvent(2.5f, [this]()
		{
			HUD->CreditsFadeOut();
		});
	TimeEventer->AddEndEvent(4.5f, [this]()
		{
			HUD->FadeOut();
		});
	TimeEventer->AddEndEvent(7.0f, [this]()
		{
			bCanNextMode = true;
		});
}

void ATitleGameMode::StartPlayGameMode()
{
	if (true == bEndInitTask)
	{
		return;
	}

	if (false == bCanNextMode)
	{
		return;
	}
	if (true == bIsSpace)
	{
		return;
	}
	if (true == UEngineInput::IsDown(VK_SPACE) && true == bIsReady)
	{
		bIsSpace = true;
		ButtonSound = UEngineSound::Play("ui_button_confirm.wav");
		HUD->FadeIn();
		TimeEventer->AddEndEvent(0.2f, [this]()
			{
				SetupPlayGameMode();
			});
	}
}

void ATitleGameMode::SetupPlayGameMode()
{
	TimeEventer->AddEndEvent(0.5f, [this]()
		{
			Volume -= 0.2f;
			Sound.SetVolume(Volume);
		});
	TimeEventer->AddEndEvent(1.0f, [this]()
		{
			Volume -= 0.2f;
			Sound.SetVolume(Volume);
		});
	TimeEventer->AddEndEvent(1.5f, [this]()
		{
			Volume -= 0.2f;
			Sound.SetVolume(Volume);
		});
	TimeEventer->AddEndEvent(2.0f, [this]()
		{
			Volume -= 0.2f;
			Sound.SetVolume(Volume);
		});
	TimeEventer->AddEndEvent(2.5f, [this]()
		{
			UEngineSound::StopAllSounds();
			PlayWorld = UEngineCore::CreateLevel<APlayGameMode, AKnight, APlayHUD>("Play").get();
			UEngineCore::OpenLevel("Play");
		});
}

void ATitleGameMode::StartLoadPlayLevelResource()
{
	if (true == bTitleLoadFinished && true == bEndInitTask)
	{
		bEndInitTask = false;

		UEngineCore::EndTime = ::clock();
		bIsExecute = true;

		TitleScene = GetWorld()->SpawnActor<ATitleScene>();
		TitleScene->SetActorLocation({ 0.0f, 0.0f, 0.0f });
		std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
		Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });

		HUD = dynamic_cast<ATitleHUD*>(GetWorld()->GetHUD());

		InitBackgroundSound();
		FadeEffect();

		PlayStartLoadingTime = ::clock();
		if (true == Thread.Joinable())
		{
			Thread.Join();
		}
		Thread.Start("PlayLoading", [this]()
			{
				UContentsResource::LoadPlayResource();
			});
	}
}

void ATitleGameMode::CheckPlayLoadComplete()
{
	if (true == UEngineCore::GetThreadPool().IsIdle() && false == bIsReady)
	{
		EndLoadingTime = ::clock();
		bIsPlayStart = true;

		Thread.Join();

		bIsReady = true;
		std::cout << std::endl;
		std::cout << "PlayLevel Resources is Ready." << std::endl;
	}
}

void ATitleGameMode::StartLevel()
{

}

void ATitleGameMode::EndLevel()
{

}

