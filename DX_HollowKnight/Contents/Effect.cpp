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

	
	// �������� ���� �κ�
	float FrameTime = 0.04f;
	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 10, FrameTime, false);


	float EffectTime = 0.05f;
	std::string Png = ".png";
	std::string Explode = "Explode";
	BodyRenderer->CreateAnimation(Explode, Explode + Png, 0, 12, EffectTime, false);

	std::string Puff = "Puff";
	BodyRenderer->CreateAnimation(Puff, Puff + Png, 0, 8, EffectTime, false);

	BodyRenderer->ChangeAnimation(FocusEffect);
}

AEffect::~AEffect()
{
	bIsValid = false;
}

void AEffect::ChangeAnimation(std::string_view _AnimationName)
{
	BodyRenderer->ChangeAnimation(_AnimationName);
}

// ��ġ ���� ���ҰŸ� ȣ��
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
	Knight = AKnight::GetPawn();
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
		CheckDirection(); // ��������, ���������� ����ϰ�

		if (nullptr != TargetActor)
		{
			SrcPos = TargetActor->GetActorLocation();
		}

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
