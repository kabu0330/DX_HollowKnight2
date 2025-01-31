#pragma once
#include "Skill.h"

// 설명 : 충돌체 생성
class AMonsterSkill : public ASkill
{
public:
	// constrcuter destructer
	AMonsterSkill();
	~AMonsterSkill();

	// delete Function
	AMonsterSkill(const AMonsterSkill& _Other) = delete;
	AMonsterSkill(AMonsterSkill&& _Other) noexcept = delete;
	AMonsterSkill& operator=(const AMonsterSkill& _Other) = delete;
	AMonsterSkill& operator=(AMonsterSkill&& _Other) noexcept = delete;

	void SetCollisionTime(float _AttackTime)
	{
		CollisionDuration = _AttackTime;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool bIsCollision = false;
	float CollisionDuration = 0.5f;

private:
	void SetCollisionEvent() override;
	void Collide(class UCollision* _This, class UCollision* _Other);
	void CreateHitEffect(class UCollision* _This, class UCollision* _Other);
	void Attack(class UCollision* _This, class UCollision* _Other);
	void Knockback();
	

};

