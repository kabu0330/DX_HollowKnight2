#pragma once
#include "Skill.h"
#include "KnightEffect.h"

// 설명 : 충돌체 + 이펙트
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
	void Collide(class UCollision* _This, class UCollision* _Other);

	virtual void CreateHitEffect(class UCollision* _This, class UCollision* _Other);
	virtual void Attack(class UCollision* _This, class UCollision* _Other);
	void CreateOrangeParticleEffect(class UCollision* _This, class UCollision* _Other);
	void CreateWhiteHitParticleEffect(class UCollision* _This, class UCollision* _Other);
	void CreateHitOrangeEffect(class UCollision* _This, class UCollision* _Other);
	void Knockback(class UCollision* _This, class UCollision* _Other);
	
private:
	FVector KnightPos = { 0.0f, 0.0f, 0.0f };
	class UTimeEventComponent* TimeEventer = nullptr;
	bool bIsParticle = false;
	inline static int Count = 0;
};

