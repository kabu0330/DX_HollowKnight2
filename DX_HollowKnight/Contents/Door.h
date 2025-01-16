#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ADoor : public AActor
{
public:
	// constrcuter destructer
	ADoor();
	~ADoor();

	// delete Function
	ADoor(const ADoor& _Other) = delete;
	ADoor(ADoor&& _Other) noexcept = delete;
	ADoor& operator=(const ADoor& _Other) = delete;
	ADoor& operator=(ADoor&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetWarpPosition(class ARoom* _TargetRoom, FVector _TargetPos, bool _IsEnter);
	void SetScale(FVector _Scale);
	void Warp(class UCollision* _Actor1, class UCollision* _Actor2);
	void EnterDoor()
	{
		bIsDoor = false;
	}
	void EnterDoorEnd()
	{
		bIsDoor = true;
	}
	UCollision* GetCollision()
	{
		return BodyCollision.get();
	}

protected:
	void ActiveKnightCollision();
private:
	std::shared_ptr<class UCollision> BodyCollision;
	std::shared_ptr<class UTimeEventComponent> TimeEventor;
	FVector Scale;
	FVector TargetPos = FVector::ZERO;
	FVector InitPos = FVector::ZERO;
	class ARoom* TargetRoom = nullptr;
	bool bIsDoor = true;

	class AKnight* Knight = nullptr;
};

