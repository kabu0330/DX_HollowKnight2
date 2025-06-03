#pragma once
#include "KnightSkill.h"

// Ό³Έν :
class AKnightSlash : public AKnightSkill
{
public:
	AKnightSlash();
	~AKnightSlash();

protected:

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void CreateHitEffect(class UCollision* _This, class UCollision* _Other) override;
	void Attack(class UCollision* _This, class UCollision* _Other) override;
	void Knockback(class UCollision* _This, class UCollision* _Other) override;


private:
	// delete Function
	AKnightSlash(const AKnightSlash& _Other) = delete;
	AKnightSlash(AKnightSlash&& _Other) noexcept = delete;
	AKnightSlash& operator=(const AKnightSlash& _Other) = delete;
	AKnightSlash& operator=(AKnightSlash&& _Other) noexcept = delete;
};

