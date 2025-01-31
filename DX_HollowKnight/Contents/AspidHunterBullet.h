#pragma once
#include "MonsterProjectile.h"

// Ό³Έν :
class AAspidHunterBullet : public AMonsterProjectile
{
public:
	// constrcuter destructer
	AAspidHunterBullet();
	~AAspidHunterBullet();

	// delete Function
	AAspidHunterBullet(const AAspidHunterBullet& _Other) = delete;
	AAspidHunterBullet(AAspidHunterBullet&& _Other) noexcept = delete;
	AAspidHunterBullet& operator=(const AAspidHunterBullet& _Other) = delete;
	AAspidHunterBullet& operator=(AAspidHunterBullet&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

