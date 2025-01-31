#include "PreCompile.h"
#include "MonsterSkill.h"
#include "FightUnit.h"

AMonsterSkill::AMonsterSkill()
{
	SetName("MonsterSkill");
	Collision->SetCollisionProfileName("MonsterObject");

	std::string BlanckEffect = "BlanckEffect";
	BodyRenderer->CreateAnimation(BlanckEffect, "SlashEffect", 0, 0, CollisionDuration, false);
	BodyRenderer->ChangeAnimation("BlanckEffect");
}

AMonsterSkill::~AMonsterSkill()
{
}

void AMonsterSkill::BeginPlay()
{
	ASkill::BeginPlay();
	
}

void AMonsterSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
}

void AMonsterSkill::SetCollisionEvent()
{
	Collision->SetCollisionEnter(std::bind(&AMonsterSkill::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

void AMonsterSkill::Collide(UCollision* _This, UCollision* _Other)
{
	CreateHitEffect(_This, _Other);
	Attack(_This, _Other);
	bIsCollision = true;
}

void AMonsterSkill::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
	UEngineDebug::OutPutString("Create Monster Effect!!!");
}

void AMonsterSkill::Attack(UCollision* _This, UCollision* _Other)
{
	if (nullptr == Knight)
	{
		return;
	}

	int MonsterAtt = 1;
	UFightUnit::OnHit(Knight, MonsterAtt);

	UEngineDebug::OutPutString("몬스터가 나이트에게 데미지를 주었습니다." );

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			Knockback();
		});
}

void AMonsterSkill::Knockback()
{
	FVector Pos = { GetActorLocation().X, GetActorLocation().Y };
	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector KnockbackDirection = KnightPos - Pos;
	KnockbackDirection.Y = 0.0f;
	KnockbackDirection.Normalize();
	KnockbackDirection += FVector::UP;
	KnockbackDirection.Normalize();

	Knight->GetStatRef().SetKnockbackDistance(500.0f);
	Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);
}

