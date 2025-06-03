#pragma once
#include "MonsterSkill.h"
#include <EnginePlatform/EngineSound.h>

// ���� :
class AMonsterProjectile : public AMonsterSkill
{
public:
	AMonsterProjectile();
	~AMonsterProjectile();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	void Release() override;
	void ChangePixelCollisionAnimaion();
	USoundPlayer Sound;

private:
	// delete Function
	AMonsterProjectile(const AMonsterProjectile& _Other) = delete;
	AMonsterProjectile(AMonsterProjectile&& _Other) noexcept = delete;
	AMonsterProjectile& operator=(const AMonsterProjectile& _Other) = delete;
	AMonsterProjectile& operator=(AMonsterProjectile&& _Other) noexcept = delete;
};

