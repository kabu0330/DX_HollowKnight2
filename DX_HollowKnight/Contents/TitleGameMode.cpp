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
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	AActor::BeginPlay();
	Sound = UEngineSound::Play("Title.mp3");
}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	if (UEngineInput::IsDown(VK_SPACE))
	{
		Sound = UEngineSound::Play("ui_button_confirm.wav");
		LevelChangeEnd();
	}
}

void ATitleGameMode::LevelChangeStart()
{
}

void ATitleGameMode::LevelChangeEnd()
{
	UEngineCore::OpenLevel("Play");
}

