#include "PreCompile.h"
#include "PlayGameMode.h"
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "Room.h"
#include "CollisionManager.h"
#include "RoomManager.h"
#include "DebugWindowGUI.h"

std::shared_ptr<ACameraActor> APlayGameMode::Camera = nullptr;
FVector APlayGameMode::MousePos = { 0.0f, 0.0f, 0.0f };
FVector APlayGameMode::KnightPos = { 0.0f, 0.0f, 0.0f };

APlayGameMode::APlayGameMode()
{
	SetCamera();
	CreateAndLinkCollisionGroup();
	URoomManager::CreateAndLinkRoom(this);
}

void APlayGameMode::SetCamera()
{
	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, 1.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
	//Camera->GetCameraComponent()->SetProjectionType(EProjectionType::Perspective);
}

void APlayGameMode::CreateAndLinkCollisionGroup()
{
	UCollisionManager::CreateCollisionProfile(this);
	UCollisionManager::LinkCollision(this);
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CheckInfo();
}

void APlayGameMode::LevelChangeStart()
{
	{
		std::shared_ptr<UDebugWindowGUI> Window = UEngineGUI::FindGUIWindow<UDebugWindowGUI>("DebugWindow");

		if (nullptr == Window)
		{
			Window = UEngineGUI::CreateGUIWindow<UDebugWindowGUI>("DebugWindow");
		}

		Window->SetActive(true);
	}
}

void APlayGameMode::CheckInfo()
{
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

APlayGameMode::~APlayGameMode()
{
}