#include "PreCompile.h"
#include "ActorComponent.h"
#include "Actor.h"

UActorComponent::UActorComponent()
{
}

UActorComponent::~UActorComponent()
{
}

bool UActorComponent::IsActive()
{
	if (nullptr == GetActor())
	{
		MSGASSERT("액터 컴포넌트는 액터 생성자에서 CreateDefaultSubobject<>()로 생성해야 합니다.");
		return false;
	}

	return UObject::IsActive() && GetActor()->IsActive();
}

bool UActorComponent::IsDestroy()
{
	return UObject::IsDestroy() || GetActor()->IsDestroy();
}

class AActor* UActorComponent::GetActor()
{
	return Actor;
}

ULevel* UActorComponent::GetWorld()
{
	return Actor->GetWorld();
}
