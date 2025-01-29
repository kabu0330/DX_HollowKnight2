#pragma once
#include <EngineCore/Actor.h>
#include "Global.h"
#include "ContentsRenderer.h"

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
	void ChangeAnimation(std::string_view _AnimationName, FVector _Pos)
	{
		ChangeAnimation(_AnimationName);
		SrcPos = _Pos;
	}
	virtual void EndFunction() {} // 렌더러 디스트로이 직전 함수 호출


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

	void SetLocation(FVector _Pos)
	{
		SrcPos = _Pos;
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
		ScaleRatio = _Ratio;
		BodyRenderer->SetAutoScaleRatio(_Ratio);
	}
	void SetScaleFade(float _ReductionRate)
	{
		bIsAutoRelease = false; // 애니메이션 시간이 아닌 크기 비율이 0이하가 되면 소멸
		bIsScaleDecay = true;
		ReductionRate = _ReductionRate;
	}

	void SetAutoRelease(bool _bIsAutoRelease)
	{
		bIsAutoRelease = _bIsAutoRelease;
	}

	bool bIsValid = true;

protected:
	AKnight* Knight = nullptr;
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
	bool bIsAutoRelease = true;

	float ZSort = 0.0f;
	std::shared_ptr<UContentsRenderer> BodyRenderer;
private:

	float ScaleRatio = 0.0f; // 파티클 옵션
	bool bIsScaleDecay = false;
	float ReductionRate = 0.0f;
	void SetScaleDecay(float _DeltaTime);
};

