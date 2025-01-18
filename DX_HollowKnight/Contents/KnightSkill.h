#pragma once
#include "Skill.h"
#include "KnightEffect.h"

// Ό³Έν :
class AKnightSkill : public ASkill
{
public:
	// constrcuter destructer
	AKnightSkill();
	~AKnightSkill();

	// delete Function
	AKnightSkill(const AKnightSkill& _Other) = delete;
	AKnightSkill(AKnightSkill&& _Other) noexcept = delete;
	AKnightSkill& operator=(const AKnightSkill& _Other) = delete;
	AKnightSkill& operator=(AKnightSkill&& _Other) noexcept = delete;

protected:

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetCollisionEvent() override;

private:

	FVector KnightPos = { 0.0f, 0.0f, 0.0f };
};

