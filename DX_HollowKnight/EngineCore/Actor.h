#pragma once
#include "SceneComponent.h"
#include <EngineCore/EngineCore.h>

// 설명 : 
class AActor : public UObject
{
	friend class ULevel;

public:
	// constrcuter destructer
	ENGINEAPI AActor();
	ENGINEAPI ~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	ENGINEAPI virtual void BeginPlay();
	ENGINEAPI virtual void Tick(float _DeltaTime);

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	template<typename ComponentType>
	inline std::shared_ptr<ComponentType> CreateDefaultSubobject()
	{
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 했습니다.");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 했습니다.");
			return nullptr;
		}

		size_t Size = sizeof(ComponentType);

		char* ComMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComPtr = reinterpret_cast<ComponentType*>(ComMemory);
		ComPtr->Actor = this;

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComMemory);
		// 레벨먼저 세팅하고
		// 플레이스먼트 new 
		std::shared_ptr<ComponentType> NewCom(new(ComMemory) ComponentType());

		AllComponentList.push_back(NewCom);

		// 내가 그냥 ActorComponent
		// 내가 그냥 SceneComponent
		if (std::is_base_of_v<UActorComponent, ComponentType> 
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewCom);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType>
			&& !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("말도 안됨");
		}

		return NewCom;
	}

	template<typename Type>
	Type* GetGameInstance()
	{
		return dynamic_cast<Type*>(GetGameInstance());
	}

	ENGINEAPI class UGameInstance* GetGameInstance();

	ULevel* GetWorld()
	{
		return World;
	}

	void SetActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetWorldLocation(_Value);
	}

	void AddActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldLocation(_Value);
	}

	void SetActorRelativeScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRelativeScale3D(_Scale);
	}

	void AddRelativeLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddRelativeLocation(_Value);
	}

	void SetActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRotation(_Value);
	}

	FVector GetActorRotation()
	{
		return RootComponent->Transform.Rotation;
	}

	void AddActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldRotation(_Value);
	}

	ENGINEAPI void AttachToActor(AActor* _Parent);

	FVector GetActorLocation()
	{
		return RootComponent->Transform.WorldLocation;
	}

	FTransform GetActorTransform()
	{
		if (nullptr == RootComponent)
		{
			return FTransform();
		}

		return RootComponent->GetTransformRef();
	}

	void SetActorTransform(const FTransform& _Transform)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->Transform = _Transform;

		return;
	}

	class USceneComponent* GetRootComponent()
	{
		return RootComponent.get();
	}


	ENGINEAPI FVector GetActorUpVector();
	ENGINEAPI FVector GetActorRightVector();
	ENGINEAPI FVector GetActorForwardVector();

	template<typename ComType>
	std::vector<std::shared_ptr<ComType>> GetComponentByClass()
	{
		std::vector<std::shared_ptr<ComType>> Result;

		for (std::shared_ptr<class UActorComponent> Component : AllComponentList)
		{
			std::shared_ptr<ComType> Com = std::dynamic_pointer_cast<ComType>(Component);
			if (nullptr != Com)
			{
				Result.push_back(Com);
			}
		}

		return Result;
	}

protected:
	std::shared_ptr<class USceneComponent> RootComponent = nullptr;

private:
	// 누구의 자식인지도 알고 
	AActor* Parent = nullptr;
	// 자기 자식들도 알게 된다.
	std::list<std::shared_ptr<AActor>> ChildList;

	ULevel* World;

	std::list<std::shared_ptr<class UActorComponent>> ActorComponentList;

	// 레퍼런스 카운트 유지용 자료구조.
	std::list<std::shared_ptr<class UActorComponent>> AllComponentList;
};

