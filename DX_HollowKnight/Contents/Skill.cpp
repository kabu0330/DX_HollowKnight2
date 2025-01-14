#include "PreCompile.h"
#include "Skill.h"
#include <EnginePlatform/EngineInput.h>

ASkill::ASkill()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = static_cast<float>(EZOrder::SKILL_FRONT);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("KnightSkill");
	Collision->GetTransformRef().Location.Z = ZSort;

}

void ASkill::CheckDirection()
{
	if (false == bIsRotation)
	{
		return;
	}
	bIsLeft = AKnight::GetPawn()->IsLeft();
}

void ASkill::SetPosition()
{
	CheckDirection(); // 왼쪽인지, 오른쪽인지 계산하고

	SrcPos = TargetActor->GetActorLocation();
	if (true == bIsLeft)
	{
		FVector OffsetPos = SrcPos + Offset;
		SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
		SetActorLocation(OffsetPos);
	}
	else
	{
		FVector OffsetPos = (SrcPos - Offset);
		SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
		SetActorLocation(OffsetPos);
	}

	SetActorRotation(Rotation); // 회전계산하고
	return;
}

void ASkill::BeginPlay()
{
	AActor::BeginPlay();

	CollisionScale = BodyRenderer->GetScale();
	Collision->SetScale3D(CollisionScale);

	Collision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			_Other->GetActor()->Destroy();
			// _Other->Destroy();
			UEngineDebug::OutPutString("Attack");
		});
}

void ASkill::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	SetPosition();
	Release();

}

void ASkill::ChangeAnimation(std::string_view _AnimationName)
{
	BodyRenderer->ChangeAnimation(_AnimationName);
}

void ASkill::Release()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}
}

void ASkill::SetZSort(int _Value)
{
	float ZSort = static_cast<float>(_Value);

}

ASkill::~ASkill()
{
}

