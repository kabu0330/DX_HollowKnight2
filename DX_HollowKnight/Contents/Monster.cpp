#include "PreCompile.h"
#include "Monster.h"


AMonster::AMonster()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;


	// Renderer
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = static_cast<float>(EZOrder::MONSTER);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	CreateAnimation();

	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D({150, 150});
	BodyCollision->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	//BodyCollision->SetActive(false);
	BodyCollision->SetCollisionProfileName("Monster");


}

AMonster::~AMonster()
{
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	CreateCollision();
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	BodyCollision;

	int a = 0;
}

void AMonster::CreateAnimation()
{
	std::string DashBug = "HuskBully";
	std::string PNG = ".png";
	//BodyRenderer->CreateAnimation(DashBug, DashBug + PNG, 0, 5, 0.2f);
	//BodyRenderer->ChangeAnimation(DashBug);

	
	//BodyRenderer->CreateAnimation()
}

void AMonster::CreateCollision()
{
	BodyCollision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			//_Other->GetActor()->Destroy();
			// _Other->Destroy();
			//UEngineDebug::OutPutString("Enter");
		});
}

