#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	~AMonster();

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	void SetOnGround(bool _Value)
	{
		bIsOnGround = _Value;
	}
	bool IsOnGround()
	{
		return bIsOnGround;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void CreateRenderer();
	void CreateCollision();

private:
	bool bIsOnGround = false;

	std::shared_ptr<class USpriteRenderer> BodyRenderer;
	std::shared_ptr<class UCollision> BodyCollision;
};

