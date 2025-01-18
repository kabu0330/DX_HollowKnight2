#include "PreCompile.h"
#include "KnightSlash.h"

AKnightSlash::AKnightSlash()
{
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

