#include "PreCompile.h"
#include "KnightSlash.h"
#include "KnightEffect.h"

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
	Collision->SetCollisionEnter([this](UCollision* _This, UCollision* _Other)
		{
			UEngineDebug::OutPutString("SlashAttack");
			AKnightEffect* Effect = GetWorld()->SpawnActor<AKnightEffect>().get();
			Effect->SetName("SlashAttack");
			Effect->SetZSort(EZOrder::SKILL_FRONT);
			AKnight* Knight = AKnight::GetPawn();
			Effect->ChangeAnimation(Knight, "NailHitEffect"); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
			Effect->SetScale(1.5f);
			AActor* Target = _Other->GetActor();
			Effect->SetLocation(Target);
		});
}

