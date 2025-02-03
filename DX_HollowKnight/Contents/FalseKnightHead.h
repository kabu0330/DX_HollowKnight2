#pragma once
#include "MonsterSkill.h"

// Ό³Έν :
class AFalseKnightHead : public AMonsterSkill
{
public:
	// constrcuter destructer
	AFalseKnightHead();
	~AFalseKnightHead();

	// delete Function
	AFalseKnightHead(const AFalseKnightHead& _Other) = delete;
	AFalseKnightHead(AFalseKnightHead&& _Other) noexcept = delete;
	AFalseKnightHead& operator=(const AFalseKnightHead& _Other) = delete;
	AFalseKnightHead& operator=(AFalseKnightHead&& _Other) noexcept = delete;

	void SetRelease()
	{
		bCanRelease = true;
	}
	void SetParent(class AFalseKnight* _Boss)
	{
		Boss = _Boss;
	}
	void SetDamage(bool _bIsDamage)
	{
		bIsDamage = _bIsDamage;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetCollisionEvent() override;
	void Collide(class UCollision* _This, class UCollision* _Other);
	void Attack();

private:
	void Release() override;
	bool bCanRelease = false;
	class AFalseKnight* Boss = nullptr;
	bool bIsDamage = false;
};

