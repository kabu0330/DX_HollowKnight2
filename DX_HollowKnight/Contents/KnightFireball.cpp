#include "PreCompile.h"
#include "KnightFireball.h"
#include "FightUnit.h"

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

void AKnightFireball::CreateHitEffect(UCollision* _This, UCollision* _Other)
{
	UEngineDebug::OutPutString("Fireball Impact");
	AKnightEffect* Effect = GetWorld()->SpawnActor<AKnightEffect>().get();
	Effect->SetName("FireballImpact");
	Effect->SetZSort(EZOrder::KNIGHT_SKILL_FRONT);
	AKnight* Knight = AKnight::GetPawn();
	Effect->ChangeAnimation(Knight, "FireballImpact"); // RootComponent�� ���ٰ� �ڲ� �����µ� ����Ʈ �־��ָ� �ȴ�.
	Effect->SetScale(1.5f);
	AActor* Target = _Other->GetActor();
	Effect->SetLocation(Target);
}

void AKnightFireball::Attack(UCollision* _This, UCollision* _Other)
{
	if (nullptr == Knight)
	{
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Other->GetActor());
	if (nullptr != Monster)
	{
		int KnightAtt = Knight->GetStatRef().GetSpellAtt();
		UFightUnit::OnHit(Monster, KnightAtt);

		int MonsterHp = Monster->GetStatRef().GetHp();
		UEngineDebug::OutPutString("����Ʈ�� ���Ϳ��� " + std::to_string(KnightAtt) + "��ŭ �������� �־����ϴ�. ���� ü�� : " + std::to_string(MonsterHp));

		Knockback(_This, _Other);
	}
}
