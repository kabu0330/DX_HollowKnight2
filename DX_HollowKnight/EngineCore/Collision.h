#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"
#include <set>

// ���� : 
class UCollision : public USceneComponent
{
public:
	ENGINEAPI UCollision();
	ENGINEAPI ~UCollision();

	ENGINEAPI void SetCollisionProfile(std::string_view _ProfileName);
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void ApplyDebugRender(UEngineCamera* _Camera, float _DeltaTime);

	std::string GetCollisionProfileName()
	{
		return ProfileName;
	}

	void SetRadius(float _Value);

	ENGINEAPI bool CheckCollision(std::string_view _OtherName, std::vector<UCollision*>& _Vector);

	ENGINEAPI bool CheckCollision(std::string_view _OtherName, FVector _NextPos, std::vector<UCollision*>& _Vector);

	void SetCollisionType(ECollisionType _Type)
	{
		CollisionType = _Type;
	}

	void CheckCollisionEvent(std::shared_ptr<UCollision> _Other);

	ENGINEAPI void SetCollisionEnter(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionStay(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionEnd(std::function<void(UCollision*, UCollision*)> _Function);

	bool IsEvent()
	{
		return Enter != nullptr || Stay != nullptr || End != nullptr;
	}

	void SetDebugColor(const FVector& _Color)
	{
		DebugColor = _Color;
	}
	FVector GetDebugColor() const
	{
		return DebugColor;
	}

	ENGINEAPI static bool& GetDebugModeRef();

private:
	static bool IsDebugMode;
	FVector DebugColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	ECollisionType CollisionType = ECollisionType::OBB2D;

	std::set<UCollision*> CollisionCheckSet;

	std::string ProfileName = "NONE";

	std::function<void(UCollision*, UCollision*)> Enter;
	std::function<void(UCollision*, UCollision*)> Stay;
	std::function<void(UCollision*, UCollision*)> End;

private:
	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;
};

