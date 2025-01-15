#include "PreCompile.h"
#include "PlayGameMode.h"
#include <EnginePlatform/EngineWinImage.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "Room.h"
#include "CollisionManager.h"
#include "RoomManager.h"
#include "DebugWindowGUI.h"

FVector APlayGameMode::MousePos = { 0.0f, 0.0f, 0.0f };
FVector APlayGameMode::KnightPos = { 0.0f, 0.0f, 0.0f };

APlayGameMode::APlayGameMode()
{
	SetCamera();
	CreateAndLinkCollisionGroup();

	Rooms.CreateAndLinkRoom(this);
}

void APlayGameMode::SetCamera()
{
	Camera = GetWorld()->GetMainCamera().get();
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
	CheckDebugInput();
}

void APlayGameMode::CheckDebugInput()
{
	if (UEngineInput::IsDown('T'))
	{
		ARoom::SwitchDebugActiveGravity();
	}
	if (UEngineInput::IsDown('P'))
	{
		GetWorld()->GetMainCamera()->FreeCameraSwitch();
	}
}

void APlayGameMode::LevelChangeStart()
{
	Window = UEngineGUI::FindGUIWindow<UDebugWindowGUI>("DebugWindow");

	if (nullptr == Window)
	{
		Window = UEngineGUI::CreateGUIWindow<UDebugWindowGUI>("DebugWindow");
	}

	Window->SetActive(true);
	
}

void APlayGameMode::CheckInfo()
{
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
	SetBasePoint(); // 원점 0, 0 표기
}

void APlayGameMode::SetBasePoint()
{ 
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BasePointCollision = CreateDefaultSubObject<UCollision>();
	BasePointCollision->SetupAttachment(RootComponent);
	BasePointCollision->SetCollisionProfileName("Door");
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);
	BasePointCollision->GetTransformRef().Location.Z = ZSort;
	BasePointCollision->SetScale3D({8, 8});
	BasePointCollision->SetDebugColor({1.0f, 0.0f, 0.0f, 1.0f });
}


APlayGameMode::~APlayGameMode()
{
}