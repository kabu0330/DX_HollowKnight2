#include "PreCompile.h"
#include "AspidHunterBullet.h"

AAspidHunterBullet::AAspidHunterBullet()
{
}

AAspidHunterBullet::~AAspidHunterBullet()
{
}

void AAspidHunterBullet::BeginPlay()
{
	AMonsterSkill::BeginPlay();
}

void AAspidHunterBullet::Tick(float _DeltaTime)
{
	AMonsterSkill::Tick(_DeltaTime);
}

