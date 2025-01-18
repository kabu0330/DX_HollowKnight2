#pragma once
#include "Effect.h"

// Ό³Έν :
class AKnightEffect : public AEffect
{
public:
	// constrcuter destructer
	AKnightEffect();
	~AKnightEffect();

	// delete Function
	AKnightEffect(const AKnightEffect& _Other) = delete;
	AKnightEffect(AKnightEffect&& _Other) noexcept = delete;
	AKnightEffect& operator=(const AKnightEffect& _Other) = delete;
	AKnightEffect& operator=(AKnightEffect&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:

};

