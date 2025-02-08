#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ABarrier : public AActor
{
public:
	// constrcuter destructer
	ABarrier();
	~ABarrier();

	// delete Function
	ABarrier(const ABarrier& _Other) = delete;
	ABarrier(ABarrier&& _Other) noexcept = delete;
	ABarrier& operator=(const ABarrier& _Other) = delete;
	ABarrier& operator=(ABarrier&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:

};

