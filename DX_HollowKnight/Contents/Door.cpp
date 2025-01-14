#include "PreCompile.h"
#include "Door.h"
#include "Room.h"

ADoor::ADoor()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetCollisionProfileName("Door");
	float ZSort = static_cast<float>(EZOrder::COLLISION);
	BodyCollision->GetTransformRef().Location.Z = ZSort;
	Scale = FVector(300, 300);
	BodyCollision->SetScale3D(Scale);

	BodyCollision->SetCollisionEnter(std::bind(&ADoor::Warp, this, std::placeholders::_1, std::placeholders::_2));
}

ADoor::~ADoor()
{
}

void ADoor::BeginPlay()
{
	AActor::BeginPlay();

	Knight = AKnight::GetPawn();
}

void ADoor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

void ADoor::SetWarpPosition(FVector _InitPos, ARoom* _TargetRoom, FVector _TargetPos, bool _IsEnter)
{
	FVector RoomPos = _TargetRoom->GetActorLocation();
	FVector RoomSize = _TargetRoom->GetSize();
	FVector RoomLeftTop = { RoomPos.X - (RoomSize.X / 2.0f), RoomPos.Y + (RoomSize.Y / 2.0f)};

	InitPos = _InitPos;
	TargetPos = RoomLeftTop + _TargetPos;
}

void ADoor::SetScale(FVector _Scale)
{
	Scale = _Scale;
	BodyCollision->SetScale3D(Scale);
}

void ADoor::Warp(UCollision* _Actor1, UCollision* _Actor2)
{
	UEngineDebug::OutPutString("Enter");
	Knight->SetActorLocation(TargetPos); 
}

