#pragma once
#include "KnightEffect.h"
#include <EngineCore/TimeEventComponent.h>

// ���� :
class KnightStunEffect : public AKnightEffect
{
public:
	KnightStunEffect();
	~KnightStunEffect();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void CreateStunImpactEffect();

protected:

private:
	UTimeEventComponent* TimeEventer = nullptr;

private:
	// delete Function
	KnightStunEffect(const KnightStunEffect& _Other) = delete;
	KnightStunEffect(KnightStunEffect&& _Other) noexcept = delete;
	KnightStunEffect& operator=(const KnightStunEffect& _Other) = delete;
	KnightStunEffect& operator=(KnightStunEffect&& _Other) noexcept = delete;
};

