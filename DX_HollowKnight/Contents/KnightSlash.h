#pragma once
#include "KnightSkill.h"

// Ό³Έν :
class AKnightSlash : public AKnightSkill
{
public:
	// constrcuter destructer
	AKnightSlash();
	~AKnightSlash();

	// delete Function
	AKnightSlash(const AKnightSlash& _Other) = delete;
	AKnightSlash(AKnightSlash&& _Other) noexcept = delete;
	AKnightSlash& operator=(const AKnightSlash& _Other) = delete;
	AKnightSlash& operator=(AKnightSlash&& _Other) noexcept = delete;

protected:

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetCollisionEvent() override;

private:

};

