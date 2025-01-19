#include "PreCompile.h"
#include "KnightFireball.h"

AKnightFireball::AKnightFireball()
{
	SetName("Fireball");
	float FireballFrame = 0.1f;
	std::string Fireball = "Fireball";
	BodyRenderer->CreateAnimation(Fireball, Fireball, 0, 3, FireballFrame);
	BodyRenderer->ChangeAnimation(Fireball);
}

AKnightFireball::~AKnightFireball()
{
}

void AKnightFireball::BeginPlay()
{
	AKnightSkill::BeginPlay();
}

void AKnightFireball::Tick(float _DeltaTime)
{
	AKnightSkill::Tick(_DeltaTime);
}

void AKnightFireball::SetCollisionEvent()
{
	Collision->SetCollisionEnter([this](UCollision* _This, UCollision* _Other)
		{
			UEngineDebug::OutPutString("FireballImpact");
			AKnightEffect* Effect = GetWorld()->SpawnActor<AKnightEffect>().get();
			Effect->SetName("FireballImpact");
			Effect->SetZSort(EZOrder::KNIGHT_SKILL_FRONT);
			AKnight* Knight = AKnight::GetPawn();
			Effect->ChangeAnimation(Knight, "FireballImpact"); // RootComponent�� ���ٰ� �ڲ� �����µ� ����Ʈ �־��ָ� �ȴ�.
			Effect->SetScale(1.5f);
			AActor* Target = _Other->GetActor();
			Effect->SetLocation(Target);
		});
}
