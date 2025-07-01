#pragma once
#include "Actor.h"
#include "EngineEnums.h"

// Ό³Έν :
class ACameraActor : public AActor
{
public:
	ENGINEAPI ACameraActor();
	ENGINEAPI ~ACameraActor();

	ENGINEAPI std::shared_ptr<class UEngineCamera> GetCameraComponent()
	{
		return CameraComponent;
	}

	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void Tick(float _DeltaTime);

	ENGINEAPI FVector GetScreenMousePosToWorldPos();

	ENGINEAPI FVector GetScreenPosToWorldPos(FVector _Pos);
	ENGINEAPI FVector GetWorldPosToScreenPos(FVector _Pos);


	ENGINEAPI inline bool IsFreeCamera()
	{
		return IsFreeCameraValue;
	}

	ENGINEAPI void ActivateFreeCamera();

	ENGINEAPI void DeactivateFreeCamera();

	ENGINEAPI void SwitchFreeCamera();

	void SetFreeCameraSpeed(float _Speed)
	{
		FreeSpeed = _Speed;
	}

protected:

private:
	std::shared_ptr<class UEngineCamera> CameraComponent = nullptr;

	bool IsFreeCameraValue = false;
	ENGINEAPI void CheckFreeCamera();

	FTransform PrevTrans;
	EProjectionType PrevProjectionType = EProjectionType::Orthographic;
	float FreeSpeed = 2000.0f;

	FVector ScreenPos;
	FVector PrevScreenPos;
	float RotSpeed = 360.0f;

private:
	// delete Function
	ACameraActor(const ACameraActor& _Other) = delete;
	ACameraActor(ACameraActor&& _Other) noexcept = delete;
	ACameraActor& operator=(const ACameraActor& _Other) = delete;
	ACameraActor& operator=(ACameraActor&& _Other) noexcept = delete;
};

