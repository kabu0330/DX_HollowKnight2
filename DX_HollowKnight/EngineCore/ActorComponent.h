#pragma once
#include <EngineBase/Object.h>

#include <memory>

// Ό³Έν :
class UActorComponent : public UObject
{
	friend class AActor;

public:
	ENGINEAPI UActorComponent();
	ENGINEAPI ~UActorComponent();

	template<typename ConvertType>
	ConvertType* GetActor()
	{
		return dynamic_cast<ConvertType*>(GetActor());
	}

	ENGINEAPI class AActor* GetActor();
	ENGINEAPI virtual void InitializeComponent() {}
	ENGINEAPI virtual void BeginPlay() {}
	ENGINEAPI virtual void ComponentTick(float _DeltaTime) {};

	ULevel* GetWorld();


	ENGINEAPI bool IsActive() override;

	ENGINEAPI bool IsDestroy() override;

protected:

private:
	class AActor* Actor;

private:
	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;
};

