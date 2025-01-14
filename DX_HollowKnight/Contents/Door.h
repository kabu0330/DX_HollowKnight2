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

	void SetWarpPosition(FVector _InitPos, class ARoom* _TargetRoom, FVector _TargetPos, bool _IsEnter);
	void SetScale(FVector _Scale);
	void Warp(class UCollision* _Actor1, class UCollision* _Actor2);

protected:

private:
	std::shared_ptr<class UCollision> BodyCollision;
	FVector Scale;
	FVector TargetPos = FVector::ZERO;
	FVector InitPos = FVector::ZERO;
	class AKnight* Knight = nullptr;
};

