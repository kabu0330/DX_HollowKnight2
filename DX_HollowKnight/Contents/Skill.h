#pragma once
#include "Effect.h"

// ���� :
class ASkill : public AEffect
{
public:
	// constrcuter destructer
	ASkill();
	~ASkill();

	// delete Function
	ASkill(const ASkill& _Other) = delete;
	ASkill(ASkill&& _Other) noexcept = delete;
	ASkill& operator=(const ASkill& _Other) = delete;
	ASkill& operator=(ASkill&& _Other) noexcept = delete;

	class UCollision* GetCollision()
	{
		return Collision.get();
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void Release() override;

	std::shared_ptr<class UCollision> Collision;
	FVector CollisionScale = FVector::ZERO;
private:

};

