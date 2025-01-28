#include "PreCompile.h"
#include "KnightSkill.h"
#include "Skill.h"
#include "Particle.h"

AKnightSkill::AKnightSkill()
{
	SetName("AKnightSkill");
	Collision->SetCollisionProfileName("KnightObject");
}

AKnightSkill::~AKnightSkill()
{
}

void AKnightSkill::BeginPlay()
{
	ASkill::BeginPlay();
}

void AKnightSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
}

void AKnightSkill::CreateParticleEffect(UCollision* _This, UCollision* _Other)
{
	if (true == bIsParticle)
	{
		return; 
	}

	bIsParticle = true;
	
	AParticle* Particle = GetWorld()->SpawnActor<AParticle>().get();
	Particle->CreateParticle("Puff", 10, 0.01f, this->GetActorLocation(), 3.0f);
}

void AKnightSkill::Knockback(UCollision* _This, UCollision* _Other)
{
	FVector TargetPos = { _Other->GetWorldLocation().X, _Other->GetWorldLocation().Y };
	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector KnockbackDirection = KnightPos - TargetPos;
	KnockbackDirection.Y = 0.0f;
	KnockbackDirection.Normalize();
	Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);

	AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	if (nullptr != Monster)
	{
		Monster->GetStatRef().SetKnockbackDir(-KnockbackDirection);
		Monster->GetStatRef().SetBeingHit(true);
	}
}







