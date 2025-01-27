#include "PreCompile.h"
#include "MonsterSkill.h"
#include "FightUnit.h"

AMonsterSkill::AMonsterSkill()
{
	SetName("MonsterSkill");
	Collision->SetCollisionProfileName("MonsterObject");

}

AMonsterSkill::~AMonsterSkill()
{
}

void AMonsterSkill::BeginPlay()
{
	ASkill::BeginPlay();
	
	// ���ӽð� ������ ���� �����ڰ� �ƴ� ���� ����÷��̿���.
	std::string BlanckEffect = "BlanckEffect";
	BodyRenderer->CreateAnimation(BlanckEffect, "SlashEffect", 0, 0, CollisionDuration, false);
	BodyRenderer->ChangeAnimation("BlanckEffect");

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

	UEngineDebug::OutPutString("���Ͱ� ����Ʈ���� �������� �־����ϴ�." );

	//	Knockback(_This, _Other);
}

void AMonsterSkill::Knockback(UCollision* _This, UCollision* _Other)
{
	//FVector TargetPos = { _Other->GetWorldLocation().X, _Other->GetWorldLocation().Y };
	//FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	//FVector KnockbackDirection = KnightPos - TargetPos;
	//KnockbackDirection.Y = 0.0f;
	//KnockbackDirection.Normalize();
	//Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);

	//AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	//if (nullptr != Monster)
	//{
	//	Monster->GetStatRef().SetKnockbackDir(-KnockbackDirection);
	//	Monster->GetStatRef().SetBeingHit(true);
	//}
}

