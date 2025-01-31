#include "PreCompile.h"
#include "MonsterProjectile.h"

AMonsterProjectile::AMonsterProjectile()
{
	float FrameTime = 0.1f;
	std::string Png = ".png";

	std::string BulletFire = "BulletFire";
	BodyRenderer->CreateAnimation(BulletFire, BulletFire + Png, 0, 6, FrameTime);

	std::string Fireball = "Fireball";
	BodyRenderer->CreateAnimation(Fireball, Fireball, 0, 3, FrameTime);

	std::string HitOrange = "HitOrange";
	std::string OrangeBullet = "OrangeBullet";
	BodyRenderer->CreateAnimation(OrangeBullet, HitOrange + Png, 0, 0, FrameTime);


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
		BodyRenderer->SetActive(false);
	}

	TimeEventer->AddEndEvent(CollisionDuration, [this]()
		{
			Collision->Destroy();
			BodyRenderer->Destroy();
			Destroy();
		});
}

