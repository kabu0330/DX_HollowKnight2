#pragma once
#include "Effect.h"

// ���� :
class ASkill : public AEffect
{
public:
	ASkill();
	~ASkill();

	class UCollision* GetCollision()
	{
		return Collision.get();
	}
	void SetCollisionScale(FVector _Scale)
	{
		CollisionScale = _Scale;
	}

	void ChangeNextAnimation(std::string_view _AnimName)
	{
		bIsNextAnimation = true;
		NextAnimationName = _AnimName;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void Release() override;
	virtual void SetCollisionEvent();

	std::shared_ptr<class UCollision> Collision;
	FVector CollisionScale = FVector::ZERO;

	bool bIsNextAnimation = false;
	std::string NextAnimationName = "";

private:
	bool IsCurRoom();
	void ActivatePixelCollision();
	bool IsPixelCollision(FVector _CollisionPoint);
	
	void ChangeNextAnimation();


private:
	// delete Function
	ASkill(const ASkill& _Other) = delete;
	ASkill(ASkill&& _Other) noexcept = delete;
	ASkill& operator=(const ASkill& _Other) = delete;
	ASkill& operator=(ASkill&& _Other) noexcept = delete;
};

