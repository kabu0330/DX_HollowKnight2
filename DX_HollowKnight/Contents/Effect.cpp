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
	ZSort = static_cast<float>(EZOrder::KNIGHT_SKILL_FRONT);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	
	// 가져가서 만들 부분
	float FrameTime = 0.04f;
	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 10, FrameTime, false);


	float EffectTime = 0.1f;
	std::string Png = ".png";
	std::string Explode = "Explode";
	BodyRenderer->CreateAnimation(Explode, Explode + Png, 0, 12, EffectTime, false);

	std::string Puff = "Puff";
	BodyRenderer->CreateAnimation(Puff, Puff + Png, 0, 8, EffectTime, false);

	float WhiteHitTime = 0.6f;
	std::string WhiteHit = "WhiteHit";
	std::string WhiteHit0 = "WhiteHit0";
	std::string WhiteHit1 = "WhiteHit1";
	std::string WhiteHit2 = "WhiteHit2";
	BodyRenderer->CreateAnimation(WhiteHit0, WhiteHit + Png, 0, 0, WhiteHitTime, false);
	BodyRenderer->CreateAnimation(WhiteHit1, WhiteHit + Png, 1, 1, WhiteHitTime, false);
	BodyRenderer->CreateAnimation(WhiteHit2, WhiteHit + Png, 2, 2, WhiteHitTime, false);


	float LightTime = 0.3f;
	std::string HitOrange = "HitOrange";
	BodyRenderer->CreateAnimation(HitOrange, HitOrange + Png, 0, 0, LightTime, false);



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

// 위치 조정 안할거면 호출
void AEffect::ChangeAnimation(AActor* _Actor, std::string_view _AnimationName)
{
	SrcPos = _Actor->GetActorLocation();
	ChangeAnimation(_AnimationName);
}

void AEffect::Release()
{
	if (false == bIsAutoRelease)
	{
		return;
	}
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
		CheckDirection(); // 왼쪽인지, 오른쪽인지 계산하고

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
