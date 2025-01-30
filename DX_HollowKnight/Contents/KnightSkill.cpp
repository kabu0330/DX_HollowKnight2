#include "PreCompile.h"
#include "KnightSkill.h"
#include "Skill.h"
#include "Particle.h"
#include <EngineCore/TimeEventComponent.h>
#include "FightUnit.h"

AKnightSkill::AKnightSkill()
{
	SetName("AKnightSkill");
	Collision->SetCollisionProfileName("KnightObject");

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

AKnightSkill::~AKnightSkill()
{
}

void AKnightSkill::BeginPlay()
{
	ASkill::BeginPlay();
	SetCollisionEvent();
}

void AKnightSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
	KnightPos = AKnight::GetPawn()->GetActorLocation();
}

void AKnightSkill::SetCollisionEvent()
{
	Collision->SetCollisionEnter(std::bind(&AKnightSkill::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

void AKnightSkill::Collide(UCollision* _This, UCollision* _Other)
{
	CreateHitEffect(_This, _Other);
	Attack(_This, _Other);
	CreateOrangeParticleEffect(_This, _Other);
	CreateHitOrangeEffect(_This, _Other);
	CreateWhiteHitParticleEffect(_This, _Other);
}

void AKnightSkill::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
}

void AKnightSkill::Attack(UCollision* _This, UCollision* _Other)
{
	if (nullptr == Knight)
	{
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	if (nullptr != Monster)
	{
		int KnightAtt = Knight->GetStatRef().GetAtt();
		UFightUnit::OnHit(Monster, KnightAtt);

		int MonsterHp = Monster->GetStatRef().GetHp();
		UEngineDebug::OutPutString("나이트가 몬스터에게 " + std::to_string(KnightAtt) + "만큼 데미지를 주었습니다. 현재 체력 : " + std::to_string(MonsterHp));

		Knockback(_This, _Other);
	}
}

void AKnightSkill::CreateOrangeParticleEffect(UCollision* _This, UCollision* _Other)
{
	if (true == bIsParticle)
	{
		return; 
	}

	bIsParticle = true;
	
	AParticle* Particle = GetWorld()->SpawnActor<AParticle>().get();
	AActor* Actor = _Other->GetActor();
	FVector ActorPos = Actor->GetActorLocation();
	Particle->CreateParticle("Puff", 30, 0.01f, ActorPos);

	Particle->SetParticleOption(EParticleType::RANDOM, -400.0f, 400.0f);
}

void AKnightSkill::CreateWhiteHitParticleEffect(UCollision* _This, UCollision* _Other)
{
	AParticle* Particle0 = GetWorld()->SpawnActor<AParticle>().get();
	AParticle* Particle1 = GetWorld()->SpawnActor<AParticle>().get();
	AParticle* Particle2 = GetWorld()->SpawnActor<AParticle>().get();
	
	Particle0->CreateParticle("WhiteHit0", 4, 0.01f, KnightPos);
	Particle1->CreateParticle("WhiteHit1", 4, 0.01f, KnightPos);
	Particle2->CreateParticle("WhiteHit2", 5, 0.01f, GetActorLocation());

	Particle0->SetZOrderOffset(40.0f);
	Particle1->SetZOrderOffset(50.0f);
	Particle2->SetZOrderOffset(60.0f);

	Particle0->SetAlpha(0.9f);
	Particle1->SetAlpha(0.9f);
	Particle2->SetAlpha(0.9f);

	Particle0->SetParticleOption(EParticleType::RANDOM, -200.0f, 200.0f);
	Particle1->SetParticleOption(EParticleType::RANDOM, -300.0f, 300.0f);
	Particle2->SetParticleOption(EParticleType::RANDOM, -200.0f, 200.0f);
}

void AKnightSkill::CreateHitOrangeEffect(UCollision* _This, UCollision* _Other)
{
	AActor* Actor = _Other->GetActor();
	AMonster* Monster = dynamic_cast<AMonster*>(Actor);
	if (nullptr == Monster)
	{
		return;
	}
	FVector ActorPos = Actor->GetActorLocation();
	FVector MonsterScale = Monster->GetRenderer()->GetScale();

	AEffect* HitOrange = GetWorld()->SpawnActor<AEffect>().get();
	HitOrange->ChangeAnimation("HitOrange", ActorPos);
	HitOrange->SetZSort(EZOrder::HIT_LIGHT_EFFECT);
	HitOrange->GetRenderer()->SetAlpha(0.5f);
	HitOrange->SetLocation(Actor);
	HitOrange->SetScale(5.0f);

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







