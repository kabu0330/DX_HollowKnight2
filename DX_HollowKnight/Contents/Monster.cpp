#include "PreCompile.h"
#include "Monster.h"

AMonster::AMonster()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	CreateRenderer();

}

AMonster::~AMonster()
{
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();

}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

}

void AMonster::CreateRenderer()
{
	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = -100.0f;
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	
	//BodyRenderer->CreateAnimation()
}

void AMonster::CreateCollision()
{
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetCollisionProfileName("Monster");
	BodyCollision->SetScale3D({ 100.0f, 130.0f });
	BodyCollision->GetTransformRef().Location.Y += 50.0f;

	BodyCollision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			//_Other->GetActor()->Destroy();
			// _Other->Destroy();
			UEngineDebug::OutPutString("Enter");
		});
}

