#pragma once
#include <EngineCore/Actor.h>

// 설명 :
class AEffect : public AActor
{
public:
	// constrcuter destructer
	AEffect();
	~AEffect();

	// delete Function
	AEffect(const AEffect& _Other) = delete;
	AEffect(AEffect&& _Other) noexcept = delete;
	AEffect& operator=(const AEffect& _Other) = delete;
	AEffect& operator=(AEffect&& _Other) noexcept = delete;

	class UContentsRenderer* GetRenderer()
	{
		return BodyRenderer.get();
	}

	void ChangeAnimation(std::string_view _AnimationName);
	void ChangeAnimation(AActor* _Actor, std::string_view _AnimationName);


	virtual void Release();

	void SetZSort(int _Value);
	void SetZSort(EZOrder _Value)
	{
		SetZSort(static_cast<int>(_Value));
	}

	void SetLocation(AActor* _Actor, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		TargetActor = _Actor;
		Offset = _OffsetPos;
		Rotation = _Rotation;

		SrcPos = TargetActor->GetActorLocation();
		SetActorLocation(SrcPos);
		CheckDirection();
	}

	void AddLocation(AActor* _Actor, FVector _Speed, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		bIsAddLocation = true;
		Speed = _Speed;
		SetLocation(_Actor, _OffsetPos, _Rotation);
	}

	// 좌우 반전
	void ToggleFlip()
	{
		RotationScale = { -1.0f, 1.0f, 1.0f };
	}

	// 좌우 반전을 원치 않는다면 false
	void EnableRotation(bool _bIsRotation)
	{
		bIsRotation = _bIsRotation;
	}
	void SetScale(float _Ratio)
	{
		BodyRenderer->SetAutoScaleRatio(_Ratio);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void SetPosition();
	void CheckDirection();

	AActor* TargetActor = nullptr;
	FVector SrcPos = FVector::ZERO;
	FVector Offset = FVector::ZERO;
	FVector Rotation = FVector::ZERO;
	FVector Speed = FVector::ZERO;
	FVector CurPos = FVector::ZERO;
	FVector RotationScale = { 1.0f, 1.0f, 1.0f };
	bool bIsAddLocation = false;
	
	bool bIsLeft = true;
	bool bIsRotation = true;

	

	float ZSort = 0.0f;
	std::shared_ptr<class UContentsRenderer> BodyRenderer;
private:
};

