#pragma once
#include "Skill.h"

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

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	//void CheckKnightPos();


protected:

private:

	FVector KnightPos = { 0.0f, 0.0f, 0.0f };
};

