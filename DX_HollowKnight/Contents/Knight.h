#pragma once
#include <EngineBase/FSMStateManager.h>
#include <EngineCore/Pawn.h>
#include "Global.h"
#include <EngineCore/TimeEventComponent.h>
#include "StatusUnit.h"

// 설명 : Player
class AKnight : public APawn
{
public:
	// constrcuter destructer
	AKnight();
	~AKnight();

	// delete Function
	AKnight(const AKnight& _Other) = delete;
	AKnight(AKnight&& _Other) noexcept = delete;
	AKnight& operator=(const AKnight& _Other) = delete;
	AKnight& operator=(AKnight&& _Other) noexcept = delete;

	void SetCameraPos();

	static AKnight* GetPawn()
	{
		return MainPawn;
	}

	USceneComponent* GetRootComponent() const
	{
		return RootComponent.get();
	}

	UContentsRenderer* GetRenderer() const
	{
		return BodyRenderer.get();
	}

	UCollision* GetCollision()
	{
		return BodyCollision.get();
	}

	bool IsLeft()
	{
		return bIsLeft;
	}

	void CheckEnterDoor(class UCollision* _This, class UCollision* _Target);
	bool GetEnter()
	{
		return bIsEnter;
	}

	UStatusUnit& GetStatRef()
	{
		return Stat;
	}
	UFSMStateManager& GetFSMRef()
	{
		return FSM;
	}


public:
	// 픽셀 충돌 디버그
	void SwitchActiveGravity()
	{
		NoneGravity = !NoneGravity;
	}

	// 픽셀 충돌
	void SetOnGround(bool _Value)
	{
		bIsOnGround = _Value;
	}
	bool IsOnGround() const
	{
		return bIsOnGround;
	}

	void SetWallHere(bool _Value)
	{
		bIsWallHere = _Value;
	}
	bool IsWallHere() const
	{
		return bIsWallHere;
	}

	void SetCeilHere(bool _Value)
	{
		bIsCeilHere = _Value;
	}
	bool GetCeilHere() const
	{
		return bIsCeilHere;
	}

	FVector GetGravityForce() const
	{
		return GravityForce;
	}
	void SetGravityForce(FVector _GravityForce)
	{
		GravityForce = _GravityForce;
	}

	float GetJumpForce()
	{
		return JumpForce;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void TimeElapsed(float _DeltaTime); // 쿨타임 관련
	void CheckEnterButton();

	// 충돌관련
	void SetCollisionEvent();
	void Collide(class UCollision* _This, class UCollision* _Other);


	// 픽셀충돌
	void ActiveGravity();
	void ActivePixelCollsion();

private:
	// Stat
	void SetStatus();
	UStatusUnit Stat = UStatusUnit();

	bool bIsEnter = false;

	// 공격 또는 피격 동작 중일 때
	bool CanAction();
	bool CanMove();
	void RecoveryIdle();


	float HitStunDuration = 1.0f;

	void ReverseForce(float _DeltaTime);
	void Knockback(float _DeltaTime);
	bool bIsOnGround = false; // 픽셀충돌로 true / false 검사

	// Move
	void Move(float _DeltaTime);


	// Jump : Z키
	bool CanJump();
	void Jump(float _DeltaTime);
	bool bCanJump = true;
	FVector GravityForce = FVector::ZERO;
	float JumpForce = 0.0f;
	float InitJumpForce = 600.0f;

	// Slash : X키
	bool bIsShowEffect = false;
	float AttackCooldownElapsed = 0.0f;
	void CreateSlashEffect();
	void CreateUpSlashEffect();
	void CreateDownSlashEffect();

	// Dash : C키
	bool bIsDashing = false;
	bool bCanDash = true;
	bool bCanRotation = true; // 방향전환 여부, 대시 중에는 불가
	float DashCooldownElapsed = 0.0f;
	void ChangeDash();
	void Dash();
	void CreateDashEffect();

	// Spell : A키
	bool bIsFireballEffect = false;
	void CastFireball();
	void CreateFireballEffect();
	bool bIsFocusEffect = false;
	void CastFocus();
	void CreateFocusEffect();
	bool bIsFocusEndEffect = false;
	void CreateFocusEndEffect();

	// 피격 : 스턴
	bool bIsStunEffect = false;
	void CreateStunEffect();

private:
	// 디버그
	void DebugInput(float _DeltaTime);
	void DebugCamera();
	void SwitchDebugMode()
	{
		bIsDebugMode = !bIsDebugMode;
	}
	bool bIsDebugMode = false;
	bool NoneGravity = false;
	static AKnight* MainPawn;

	UTimeEventComponent* TimeEventor = nullptr;

	// 카메라
	void SetCameraLerp();
	void CheckCameraPos();
	bool bIsCameraMove = false;
	FVector CameraPos = { 0.0f, 0.0f, 0.0f };
	FVector CameraCurPos = FVector::ZERO;
	FVector CameraTargetPos = FVector::ZERO;
	float CameraMoveTime = 0.0f;
	float ScreenRatioY = 0.0f;


	// Renderer
	std::shared_ptr<class UContentsRenderer> BodyRenderer;
	void CreateRenderer();

	// Collision
	bool bIsWallHere = false;
	bool bIsCeilHere = false;
	std::shared_ptr<class UCollision> BodyCollision;
	void CreateCollision();

	// Animation
	void InitAnimation();
	void ChangeNextAnimation(EKnightState _NextState);
	void ChangePrevAnimation();

	void ChangeAttackAnimation(EKnightState _PrevState);
	void ChangeJumpAnimation();
	void ChangeNonCombatAnimation();
	void ChangeLookAnimation();

	// FSM
	using StateCallback = void(AKnight::*)(float);
	void CreateState(EKnightState _State, StateCallback _Callback, std::string_view _AnimationName);

	void SetFSM();
	UFSMStateManager FSM;
	EKnightState NextState = EKnightState::IDLE;

	void SetIdle(float _DeltaTime);
	void SetRun(float _DeltaTime);
	void SetIdleToRun(float _DeltaTime);
	void SetRunToIdle(float _DeltaTime);
	void SetDash(float _DeltaTime);
	void SetJump(float _DeltaTime);
	void SetAirborn(float _DeltaTime);
	void SetLand(float _DeltaTime);
	void SetHardLand(float _DeltaTime);

	void SetLookDown(float _DeltaTime);
	void SetLookDownLoop(float _DeltaTime);
	void SetLookUp(float _DeltaTime);
	void SetLookUpLoop(float _DeltaTime);

	void SetSlash(float _DeltaTime);
	void SetUpSlash(float _DeltaTime);
	void SetDownSlash(float _DeltaTime);

	void SetFocus(float _DeltaTime);
	void SetFocusGet(float _DeltaTime);
	void SetFocusEnd(float _DeltaTime);
	void SetFireballAntic(float _DeltaTime);
	void SetFireballCast(float _DeltaTime);

	void SetStun(float _DeltaTime);

	void SetDeath(float _DeltaTime);
	void SetDeathDamage(float _DeltaTime);
	void SetDeathHead(float _DeltaTime);

	void CheckDirection();
	bool bIsLeft = true;
};

