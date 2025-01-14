#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ASkill : public AActor
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

	class UContentsRenderer* GetRenderer()
	{
		return BodyRenderer.get();
	}
	class UCollision* GetCollision()
	{
		return Collision.get();
	}

	void ChangeAnimation(std::string_view _AnimationName);
	void Release();

	void SetZSort(int _Value);
	void SetZSort(EZOrder _Value)
	{
		SetZSort(static_cast<int>(_Value));
	}

	void SetLocation(AActor* _Actor, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		TargetActor = _Actor;

		Offset = _OffsetPos;
		Rotation = _Rotation;
	}

	void EnableRotation(bool _bIsRotation)
	{
		bIsRotation = _bIsRotation;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetPosition();
	void CheckDirection();

	AActor* TargetActor = nullptr;
	FVector SrcPos = FVector::ZERO;
	FVector Offset = FVector::ZERO;
	FVector Rotation = FVector::ZERO;
	bool bIsLeft = true;
	bool bIsRotation = true;

	std::shared_ptr<class UContentsRenderer> BodyRenderer;
	std::shared_ptr<class UCollision> Collision;
	FVector CollisionScale = FVector::ZERO;
private:

};

