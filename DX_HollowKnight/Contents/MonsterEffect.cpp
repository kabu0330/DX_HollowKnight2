#include "PreCompile.h"
#include "MonsterEffect.h"

AMonsterEffect::AMonsterEffect()
{
	float FrameTime = 0.1f;
	std::string StunEffect03 = "StunEffect03";
	BodyRenderer->CreateAnimation(StunEffect03, StunEffect03, 0, 7, FrameTime, false);

	BodyRenderer->ChangeAnimation(StunEffect03);
}

AMonsterEffect::~AMonsterEffect()
{
}

void AMonsterEffect::BeginPlay()
{
	AEffect::BeginPlay();
}

void AMonsterEffect::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
}

