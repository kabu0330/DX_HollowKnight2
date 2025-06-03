#pragma once
#include "KnightSkill.h"

// Ό³Έν :
class AKnightFireball : public AKnightSkill
{
public:
	AKnightFireball();
	~AKnightFireball();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void CreateHitEffect(class UCollision* _This, class UCollision* _Other) override;
	void Attack(class UCollision* _This, class UCollision* _Other) override;
protected:

private:
	void KnightKnockback(FVector _KnockbackDir) override {}
	FVector PointPos = FVector::ZERO;
	bool bIsEffect = false;


private:
	// delete Function
	AKnightFireball(const AKnightFireball& _Other) = delete;
	AKnightFireball(AKnightFireball&& _Other) noexcept = delete;
	AKnightFireball& operator=(const AKnightFireball& _Other) = delete;
	AKnightFireball& operator=(AKnightFireball&& _Other) noexcept = delete;
};

