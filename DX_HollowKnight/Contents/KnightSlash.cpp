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
	Effect->ChangeAnimation(Knight, "NailHitEffect"); // RootComponent�� ���ٰ� �ڲ� �����µ� ����Ʈ �־��ָ� �ȴ�.
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

	// 1. _This�� �������� ���̳��� ĳ�������� �Ǻ�
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

	// 2. _Other�� �������� ���̳��� ĳ�������� �Ǻ�
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

	// 3. nullptr üũ
	if (nullptr != Knight && nullptr != Monster)
	{
		// 4. ������ ���
		int KnightAtt = Knight->GetStatRef().GetAtt();
		Monster->GetStatRef().AddHp(-KnightAtt);
		//Monster->GetStatRef().GetHp();
		UEngineDebug::OutPutString("����Ʈ�� ���Ϳ��� " + std::to_string(KnightAtt) + "��ŭ �������� �־����ϴ�.");
	}
}

