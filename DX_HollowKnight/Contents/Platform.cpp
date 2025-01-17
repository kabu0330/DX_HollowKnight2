#include "PreCompile.h"
#include "Platform.h"

APlatform::APlatform()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// Collision
	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	Collision->SetScale3D({ 150, 50 });
	//Collision->SetCollisionType(ECollisionType::AABB);

	Collision->SetCollisionProfileName("Platform");
	Collision->SetCollisionStay(std::bind(&APlatform::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

APlatform::~APlatform()
{
}

void APlatform::BeginPlay()
{
	AActor::BeginPlay();
}

void APlatform::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

void APlatform::Collide(UCollision* _This, UCollision* _Other)
{
	AActor* Actor = _Other->GetActor();
	AKnight* Knight = dynamic_cast<AKnight*>(Actor);
	FVector HalfScale = FVector::ZERO;
	if (nullptr != Knight)
	{
		HalfScale = Knight->GetRenderer()->GetScale() * 0.5f;
	}
	FVector PlatformHalfScale = Collision->GetWorldScale3D().Half();
	FVector Pos = GetActorLocation();
	float Top = Pos.Y + PlatformHalfScale.Y;

	FVector ActorPos = GetActorLocation();
	float ActorBot = ActorPos.Y - HalfScale.Y;
	
	if (Top > ActorBot)
	{
		Actor->AddActorLocation({ 0.0f, 10.0f });
		ARoom* Room = ARoom::GetCurRoom();
		Room->SetActiveGravity(false);
	}


	int a = 0;
}

