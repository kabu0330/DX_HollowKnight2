#include "PreCompile.h"
#include "Door.h"
#include "Room.h"
#include <EngineCore/TimeEventComponent.h>

ADoor::ADoor()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetCollisionProfileName("Door");
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);
	BodyCollision->GetTransformRef().Location.Z = ZSort;
	Scale = FVector(200, 100);
	BodyCollision->SetScale3D(Scale);
	//BodyCollision->SetCollisionType(ECollisionType::AABB);

	BodyCollision->SetCollisionStay(std::bind(&ADoor::Warp, this, std::placeholders::_1, std::placeholders::_2));

	TimeEventor = CreateDefaultSubObject<UTimeEventComponent>();
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

void ADoor::SetWarpPosition(ARoom* _TargetRoom, FVector _TargetPos, bool _IsDoor)
{
	TargetPos = _TargetPos;
	TargetRoom = _TargetRoom;
	bIsDoor = _IsDoor;
}

void ADoor::SetScale(FVector _Scale)
{
	BodyCollision->SetRelativeScale3D(_Scale);
}

void ADoor::Warp(UCollision* _Actor1, UCollision* _Actor2)
{
	if (true == bIsDoor)
	{
		AActor* Actor = _Actor2->GetActor();
		AKnight* Knight = dynamic_cast<AKnight*>(Actor);
		if (nullptr == Knight || false == Knight->GetEnter())
		{
			return;
		}
	}
	UEngineDebug::OutPutString("Enter");
	Knight->SetActorLocation(TargetPos); 
	ARoom::SetCurRoom(TargetRoom); // ÇÈ¼¿Ãæµ¹ ±âÁØ ¸ÊÀ» ¹Ù²Û´Ù.
	Knight->GetCollision()->SetActive(false);
	Knight->SetCameraPos();
	TimeEventor->AddEndEvent(2.0f, std::bind(&ADoor::ActiveKnightCollision, this));
}

void ADoor::ActiveKnightCollision()
{
	Knight->GetCollision()->SetActive(true);
}

