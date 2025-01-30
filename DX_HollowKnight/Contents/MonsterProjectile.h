#pragma once
#include "MonsterSkill.h"

// Ό³Έν :
class MonsterProjectile : public AMonsterSkill
{
public:
	// constrcuter destructer
	MonsterProjectile();
	~MonsterProjectile();

	// delete Function
	MonsterProjectile(const MonsterProjectile& _Other) = delete;
	MonsterProjectile(MonsterProjectile&& _Other) noexcept = delete;
	MonsterProjectile& operator=(const MonsterProjectile& _Other) = delete;
	MonsterProjectile& operator=(MonsterProjectile&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

