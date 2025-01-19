#include "PreCompile.h"
#include "Effect.h"

AEffect::AEffect()
{
	SetName("AEffect");

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	ZSort = static_cast<float>(EZOrder::KNIGHT_SKILL_BACK);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });


	float FrameTime = 0.06f;
	float DashFrameTime = 0.03f;
	float DashFrameTime2 = 0.02f;

	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 10, FrameTime, false);




	BodyRenderer->ChangeAnimation(FocusEffect);
}

AEffect::~AEffect()
{
}

void AEffect::ChangeAnimation(std::string_view _AnimationName)
{
	BodyRenderer->ChangeAnimation(_AnimationName);
}

// 위치 조정 안할거면 호출
void AEffect::ChangeAnimation(AActor* _Actor, std::string_view _AnimationName)
{
	TargetActor = _Actor;
	ChangeAnimation(_AnimationName);
}

void AEffect::Release()
{
	if (nullptr == BodyRenderer)
	{
		Destroy();
		return;
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		UEngineDebug::OutPutString(GetName() + " Effect Destroy");
		EndFunction();
		Destroy();
		return;
	}
}

void AEffect::SetZSort(int _Value)
{
	float ZSort = static_cast<float>(_Value);
	BodyRenderer->SetWorldLocation({ GetActorLocation().X, GetActorLocation().Y, ZSort });
}

void AEffect::BeginPlay()
{
	AActor::BeginPlay();
}

void AEffect::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	SetPosition();
	Release();
}

void AEffect::SetPosition()
{
	if (false == bIsAddLocation) // Set
	{
		CheckDirection(); // 왼쪽인지, 오른쪽인지 계산하고
		SrcPos = TargetActor->GetActorLocation();
		if (true == bIsLeft || false == bIsRotation)
		{
			FVector OffsetPos = SrcPos + Offset;
			SetActorLocation(OffsetPos);
			SetActorRelativeScale3D(RotationScale);
			SetActorRotation(-Rotation);
		}
		else
		{
			FVector OffsetPos = (SrcPos - Offset);
			SetActorLocation(OffsetPos);
			SetActorRelativeScale3D({ RotationScale.X * -1.0f, RotationScale.Y, RotationScale.Z });
			SetActorRotation(Rotation);
		}
	}
	else // Add
	{
		float DeltaTime = UEngineCore::GetDeltaTime();
		if (true == bIsLeft || false == bIsRotation)
		{
			AddActorLocation(-Speed * DeltaTime);
			SetActorRelativeScale3D(RotationScale);
			SetActorRotation(-Rotation);
		}
		else
		{
			AddActorLocation(Speed * DeltaTime);
			SetActorRelativeScale3D({ RotationScale.X * -1.0f, RotationScale.Y, RotationScale.Z });
			SetActorRotation(Rotation);
		}
	}
}

void AEffect::CheckDirection()
{
	if (false == bIsRotation)
	{
		return;
	}
	bIsLeft = AKnight::GetPawn()->IsLeft();
}
