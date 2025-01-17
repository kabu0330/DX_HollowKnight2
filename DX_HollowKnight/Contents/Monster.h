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

	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}
	class ARoom* GetParentRoom()
	{
		return ParentRoom;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void CreateAnimation();
	void CreateCollision();

private:
	bool bIsOnGround = false;

	class ARoom* ParentRoom = nullptr;

	std::shared_ptr<class UContentsRenderer> BodyRenderer;
	std::shared_ptr<class UCollision> BodyCollision;
};

