#include "PreCompile.h"
#include "MonsterProjectile.h"

MonsterProjectile::MonsterProjectile()
{
	float FrameTime = 0.1f;
	std::string StunEffect03 = "StunEffect03";
	BodyRenderer->CreateAnimation(StunEffect03, StunEffect03, 0, 7, FrameTime, false);

	BodyRenderer->ChangeAnimation(StunEffect03);
}

MonsterProjectile::~MonsterProjectile()
{
}

void MonsterProjectile::BeginPlay()
{
	AMonsterSkill::BeginPlay();
}

void MonsterProjectile::Tick(float _DeltaTime)
{
	AMonsterSkill::Tick(_DeltaTime);

}

