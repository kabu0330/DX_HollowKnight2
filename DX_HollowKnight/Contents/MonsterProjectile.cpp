#include "PreCompile.h"
#include "MonsterProjectile.h"

AMonsterProjectile::AMonsterProjectile()
{
	float FrameTime = 0.1f;
	std::string Png = ".png";
	std::string BulletFire = "BulletFire";
	BodyRenderer->CreateAnimation(BulletFire, BulletFire + Png, 0, 6, FrameTime);
	BodyRenderer->SetAutoScale(false);
	BodyRenderer->SetRelativeScale3D({ 100.0f, 100.0f });

	BodyRenderer->ChangeAnimation(BulletFire);
}

AMonsterProjectile::~AMonsterProjectile()
{
}

void AMonsterProjectile::BeginPlay()
{
	AMonsterSkill::BeginPlay();
}

void AMonsterProjectile::Tick(float _DeltaTime)
{
	AMonsterSkill::Tick(_DeltaTime);

}

void AMonsterProjectile::Release()
{
	if (true == bIsCollision)
	{
		Collision->SetActive(false);
	}
}

