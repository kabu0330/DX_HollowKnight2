#include "PreCompile.h"
#include "KnightSlash.h"
#include "KnightEffect.h"
#include <EngineBase/EngineRandom.h>
#include "FightUnit.h"

AKnightSlash::AKnightSlash()
{
	SetName("Knight Slash");

	float FrameTime = 0.03f;

	std::string SlashEffect = "SlashEffect";
	BodyRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, FrameTime, false);

	std::string UpSlashEffect = "UpSlashEffect";
	BodyRenderer->CreateAnimation(UpSlashEffect, UpSlashEffect, 0, 5, FrameTime, false);

	std::string DownSlashEffect = "DownSlashEffect";
	BodyRenderer->CreateAnimation(DownSlashEffect, DownSlashEffect, 0, 5, FrameTime, false);

	BodyRenderer->ChangeAnimation(SlashEffect);
}

AKnightSlash::~AKnightSlash()
{
}

void AKnightSlash::BeginPlay()
{
	AKnightSkill::BeginPlay();
}

void AKnightSlash::Tick(float _DeltaTime)
{
	AKnightSkill::Tick(_DeltaTime);
}

void AKnightSlash::SetCollisionEvent()
{
	Collision->SetCollisionEnter(std::bind(&AKnightSlash::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

void AKnightSlash::Collide(class UCollision* _This, class UCollision* _Other)
{
	CreateHitEffect(_This, _Other);
	Attack(_This, _Other);
}

void AKnightSlash::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
	UEngineDebug::OutPutString("SlashAttack");

	AKnightEffect* Effect = GetWorld()->SpawnActor<AKnightEffect>().get();
	Effect->SetName("SlashAttack");
	Effect->SetZSort(EZOrder::KNIGHT_SKILL_FRONT);
	Effect->ChangeAnimation(Knight, "NailHitEffect"); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
	Effect->SetScale(1.5f);
	AActor* Target = _Other->GetActor(); // Monster

	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector MonsterPos = { Target->GetActorLocation().X, Target->GetActorLocation().Y };
	FVector Direction = KnightPos - MonsterPos;

	Direction.Normalize();
	float Dir = Direction.Length();
	AMonster* Monster = dynamic_cast<AMonster*>(Target);
	if (nullptr == Monster)
	{
		return;
	}
	FVector Offset = Monster->GetRenderer()->GetRealScale().Half();
	if (UEngineString::ToUpper("SlashEffect") == BodyRenderer->GetCurSpriteName())
	{
		Offset.Y = 0.0f;
	}
	Effect->SetLocation(Target, Offset * Dir);

	UEngineRandom Random;
	float Degree = Random.Randomfloat(0.0f, 360.0f);
	FVector Rotation = { 0.0f, 0.0f, Degree };
	Effect->GetRenderer()->SetRotation(Rotation);
}

void AKnightSlash::Attack(UCollision* _This, UCollision* _Other)
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
		UEngineDebug::OutPutString("나이트가 몬스터에게 " + std::to_string(KnightAtt) + "만큼 데미지를 주었습니다. 현재 체력 : " + std::to_string(MonsterHp) );

		Knockback(_This, _Other);
	}
}

void AKnightSlash::Knockback(UCollision* _This, UCollision* _Other)
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

