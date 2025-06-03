#pragma once
#include "Effect.h"

// ���� : �浹ü ���� ����Ʈ�� ����
class AKnightEffect : public AEffect
{
public:
	AKnightEffect();
	~AKnightEffect();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	// delete Function
	AKnightEffect(const AKnightEffect& _Other) = delete;
	AKnightEffect(AKnightEffect&& _Other) noexcept = delete;
	AKnightEffect& operator=(const AKnightEffect& _Other) = delete;
	AKnightEffect& operator=(AKnightEffect&& _Other) noexcept = delete;
};

