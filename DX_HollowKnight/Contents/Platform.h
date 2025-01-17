#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class APlatform : public AActor
{
public:
	// constrcuter destructer
	APlatform();
	~APlatform();

	// delete Function
	APlatform(const APlatform& _Other) = delete;
	APlatform(APlatform&& _Other) noexcept = delete;
	APlatform& operator=(const APlatform& _Other) = delete;
	APlatform& operator=(APlatform&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void SetScale(FVector _Scale)
	{
		Collision->SetScale3D(_Scale);
		float ZSort = static_cast<float>(EZOrder::BACKGROUND);
		Collision->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	}

	void Collide(class UCollision* _This, class UCollision* _Other);

protected:

private:
	std::shared_ptr<class UCollision> Collision;
};

