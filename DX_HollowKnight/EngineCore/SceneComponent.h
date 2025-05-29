#pragma once
#include <EngineBase/EngineMath.h>
#include "ActorComponent.h"
#include "TransformObject.h"

// Ό³Έν :
class USceneComponent : public UActorComponent, public UTransformObject
{
	friend class AActor;

public:
	USceneComponent();
	virtual ~USceneComponent() = 0;

	ENGINEAPI void ComponentTick(float _DeltaTime) override;

protected:
	ENGINEAPI void BeginPlay() override;

private:
	// delete Function
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;
};

