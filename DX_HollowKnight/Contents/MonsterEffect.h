#pragma once
#include "Effect.h"

// 설명 : 충돌체 없이 이펙트만 생성
class AMonsterEffect : public AEffect
{
public:
	AMonsterEffect();
	~AMonsterEffect();

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	// delete Function
	AMonsterEffect(const AMonsterEffect& _Other) = delete;
	AMonsterEffect(AMonsterEffect&& _Other) noexcept = delete;
	AMonsterEffect& operator=(const AMonsterEffect& _Other) = delete;
	AMonsterEffect& operator=(AMonsterEffect&& _Other) noexcept = delete;
};

