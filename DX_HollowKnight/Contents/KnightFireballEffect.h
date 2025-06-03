#pragma once
#include "KnightEffect.h"

// Ό³Έν :
class AKnightFireballEffect : public AKnightEffect
{
public:
	AKnightFireballEffect();
	~AKnightFireballEffect();

protected:

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	// delete Function
	AKnightFireballEffect(const AKnightFireballEffect& _Other) = delete;
	AKnightFireballEffect(AKnightFireballEffect&& _Other) noexcept = delete;
	AKnightFireballEffect& operator=(const AKnightFireballEffect& _Other) = delete;
	AKnightFireballEffect& operator=(AKnightFireballEffect&& _Other) noexcept = delete;
};

