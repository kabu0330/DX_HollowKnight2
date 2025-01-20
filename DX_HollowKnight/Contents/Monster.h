#pragma once
#include <EngineBase/FSMStateManager.h>
#include <EngineCore/Actor.h>
#include "Global.h"
#include "StatusUnit.h"

// ���� :
class AMonster : public AActor
{
public:
	// constrcuter destructer
	AMonster();
	~AMonster() {};

	// delete Function
	AMonster(const AMonster& _Other) = delete;
	AMonster(AMonster&& _Other) noexcept = delete;
	AMonster& operator=(const AMonster& _Other) = delete;
	AMonster& operator=(AMonster&& _Other) noexcept = delete;

	void SetPause();

	bool IsLeft()
	{
		return bIsLeft;
	}

	UContentsRenderer* GetRenderer() const
	{
		return BodyRenderer.get();
	}

	UCollision* GetCollision()
	{
		return BodyCollision.get();
	}

	void SetParentRoom(class ARoom* _ParentRoom)
	{
		ParentRoom = _ParentRoom;
	}
	class ARoom* GetParentRoom()
	{
		return ParentRoom;
	}

	UStatusUnit& GetStatRef()
	{
		return Stat;
	}
	UFSMStateManager& GetFSMRef()
	{
		return FSM;
	}

	// ���� ����
	bool IsPlayerNearby();
	FVector GetDirectionToPlayer();
	FVector GetRandomDirection();

protected:
	virtual void SetStatus();
	UStatusUnit Stat = UStatusUnit();
	class AKnight* Knight = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// �浹����
	void SetCollisionEvent();
	void OnBodyCollision(class UCollision* _This, class UCollision* _Other);
	void CancleCurAction();

	void Move(float _DeltaTime);
	void TimeElapsed(float _DeltaTime);
	void DebugInput(float _DeltaTime);

	// ���� �Ǵ� �ǰ� ���� ���� ��
	bool bIsAttacking = false;
	bool bIsBeingHit = false;
	bool bIsStun = false;
	bool bIsDeath = false;

	// ��Ÿ ����
	float HitStunDuration = 1.0f;

	float JumpForce = 0.0f;
	float InitJumpForce = 600.0f;


	// ����
	bool bCanJump = false;
	bool bCanRotation = true; // ���� ���� ��ȯ ������ Ÿ�̹��ΰ�

	class ARoom* ParentRoom = nullptr;
	class ARoom* CurRoom = nullptr; 

	void CreateAnimation();
	void CreateCollision();
	void CreateCenterPoint();
	void CreateDetectCollision();
	FVector RendererOffset = FVector::ZERO;
	FVector BodyCollisionOffset = FVector::ZERO;
	FVector DetectRange = FVector::ZERO;
	float ZSort = 0.0f;
	std::shared_ptr<class UContentsRenderer> BodyRenderer;
	std::shared_ptr<class UCollision> BodyCollision;
	std::shared_ptr<class UCollision> CenterPoint;
	std::shared_ptr<class UCollision> DetectCollision;

	void ChangeNextAnimation(EMonsterState _NextState);
	void ChangePrevAnimation();

protected:
	bool IsCurRoom();
	virtual bool CanAction();
	bool CanJump();


	using StateCallback = void(AMonster::*)(float);
	void CreateState(EMonsterState _State, StateCallback _Callback, std::string_view _AnimationName);
	
	void SetFSM();
	UFSMStateManager FSM;
	EMonsterState NextState = EMonsterState::IDLE;
	// �̵�
	virtual void SetIdle(float _DeltaTime);
	virtual void SetWalk(float _DeltaTime);
	virtual void SetRun(float _DeltaTime);
	virtual void SetTurn(float _DeltaTime);

	// ����
	virtual void SetJumpAnticipate(float _DeltaTime);
	virtual void SetJump(float _DeltaTime);
	virtual void SetJumpAttackAnticipate(float _DeltaTime);
	virtual void SetJumpAttack(float _DeltaTime);
	virtual void SetLand(float _DeltaTime);
	virtual void SetHardLand(float _DeltaTime);

	// ���
	virtual void SetDashArrive(float _DeltaTime);
	virtual void SetDashAnticipate(float _DeltaTime);
	virtual void SetDash(float _DeltaTime);
	virtual void SetDashAttack(float _DeltaTime);
	virtual void SetDashRecovery(float _DeltaTime);

	// ����
	virtual void SetAttackAnticipate(float _DeltaTime);
	virtual void SetAttack(float _DeltaTime);
	virtual void SetAttackRecovery(float _DeltaTime);

	virtual void SetThrowAnticipate(float _DeltaTime);
	virtual void SetThrow(float _DeltaTime);
	virtual void SetThrowRecovery(float _DeltaTime);

	virtual void SetWallAnticipate(float _DeltaTime);
	virtual void SetWall(float _DeltaTime);
	virtual void SetWallAttack(float _DeltaTime);
	virtual void SetWallRecovery(float _DeltaTime);

	virtual void SetCounterAnticipate(float _DeltaTime);
	virtual void SetCounterStance(float _DeltaTime);
	virtual void SetCounter(float _DeltaTime);
	virtual void SetCounterAttack(float _DeltaTime);
	virtual void SetCounterAttackRecovery(float _DeltaTime);
	virtual void SetCounterRecovery(float _DeltaTime);

	// ���
	virtual void SetShieldAnticipate(float _DeltaTime);
	virtual void SetShieldFront(float _DeltaTime);
	virtual void SetShieldFrontBump(float _DeltaTime);
	virtual void SetShieldTop(float _DeltaTime);
	virtual void SetShieldTopBump(float _DeltaTime);

	// ȸ��
	virtual void SetEvadeAnticipate(float _DeltaTime);
	virtual void SetEvade(float _DeltaTime);

	// �ǰ�
	virtual void SetHit(float _DeltaTime);
	virtual void SetStun(float _DeltaTime);
	virtual void SetStunRoll(float _DeltaTime);
	virtual void SetStunRollEnd(float _DeltaTime);
	virtual void SetStunOpen(float _DeltaTime);
	virtual void SetStunHit(float _DeltaTime);

	// ���
	virtual void SetDeathAir(float _DeltaTime);
	virtual void SetDeathLand(float _DeltaTime);

	// ����
	virtual void SetFly(float _DeltaTime);
	virtual void SetFire(float _DeltaTime);
	virtual void SetBurst(float _DeltaTime);


	void CheckDirection();
	bool bIsLeft = true;
	FVector Direction = FVector::LEFT;
	bool bCanFly = false;

public:
		// �ȼ� �浹
		void SetOnGround(bool _Value)
		{
			bIsOnGround = _Value;
		}
		bool IsOnGround()
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

private:
	// �ȼ� �浹
	bool bIsOnGround = false;
	bool bIsWallHere = false;
	bool bIsCeilHere = false;
	FVector GravityForce = FVector::ZERO;
	FVector GravityPointOffset = FVector::DOWN; // �������� �̹��� ��ġ�� ���̰� �� ���
	FVector WallPointOffest = FVector::ZERO;
	FVector CeilPointOffset = FVector::ZERO;	

	void CheckCurRoom();
	void ActivePixelCollision();
	bool bIsPause = false;
};

