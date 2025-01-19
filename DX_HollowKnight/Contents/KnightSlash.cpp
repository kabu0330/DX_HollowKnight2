#include "PreCompile.h"
#include "KnightSlash.h"
#include "KnightEffect.h"
#include <EngineBase/EngineRandom.h>

AKnightSlash::AKnightSlash()
{
	SetName("Knight Slash");
	Collision->SetCollisionProfileName("KnightObject");

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
	AKnight* Knight = AKnight::GetPawn();
	Effect->ChangeAnimation(Knight, "NailHitEffect"); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
	Effect->SetScale(1.5f);
	AActor* Target = _Other->GetActor();
	Effect->SetLocation(Target);

	UEngineRandom Random;
	float Degree = Random.Randomfloat(0.0f, 360.0f);
	FVector Rotation = { 0.0f, 0.0f, Degree };
	Effect->GetRenderer()->SetRotation(Rotation);
}

void AKnightSlash::Attack(UCollision* _This, UCollision* _Other)
{
	AKnight* Knight = nullptr;
	AMonster* Monster = nullptr;

	// 1. _This가 누구인지 다이나믹 캐스팅으로 판별
	AActor* ThisActor = _This->GetActor();
	AKnight* KnightActor = dynamic_cast<AKnight*>(ThisActor);
	if (nullptr != KnightActor)
	{
		Knight = KnightActor;
	}
	AMonster* MonsterActor = dynamic_cast<AMonster*>(ThisActor);
	if (nullptr != MonsterActor)
	{
		Monster = MonsterActor;
	}

	// 2. _Other가 누구인지 다이나믹 캐스팅으로 판별
	AActor* OtherActor = _Other->GetActor();
	KnightActor = dynamic_cast<AKnight*>(OtherActor);
	if (nullptr != KnightActor)
	{
		Knight = KnightActor;
	}
	MonsterActor = dynamic_cast<AMonster*>(OtherActor);
	if (nullptr != MonsterActor)
	{
		Monster = MonsterActor;
	}

	// 3. nullptr 체크
	if (nullptr != Knight && nullptr != Monster)
	{
		// 4. 데미지 계산
		int KnightAtt = Knight->GetStatRef().GetAtt();
		Monster->GetStatRef().AddHp(-KnightAtt);
		//Monster->GetStatRef().GetHp();
		UEngineDebug::OutPutString("나이트가 몬스터에게 " + std::to_string(KnightAtt) + "만큼 데미지를 주었습니다.");
	}
}

