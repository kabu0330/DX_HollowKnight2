#pragma once
#include "Effect.h"

// Ό³Έν :
class AFogEffect : public AEffect
{
public:
	// constrcuter destructer
	AFogEffect();
	~AFogEffect();

	// delete Function
	AFogEffect(const AFogEffect& _Other) = delete;
	AFogEffect(AFogEffect&& _Other) noexcept = delete;
	AFogEffect& operator=(const AFogEffect& _Other) = delete;
	AFogEffect& operator=(AFogEffect&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

