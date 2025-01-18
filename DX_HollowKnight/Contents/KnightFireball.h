#pragma once
#include "KnightSkill.h"

// Ό³Έν :
class AKnightFireball : public AKnightSkill
{
public:
	// constrcuter destructer
	AKnightFireball();
	~AKnightFireball();

	// delete Function
	AKnightFireball(const AKnightFireball& _Other) = delete;
	AKnightFireball(AKnightFireball&& _Other) noexcept = delete;
	AKnightFireball& operator=(const AKnightFireball& _Other) = delete;
	AKnightFireball& operator=(AKnightFireball&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetCollisionEvent() override;

protected:

private:

};

