#pragma once
#include "Monster.h"

// 설명 :
class AFalseKnight : public AMonster
{
public:
	// constrcuter destructer
	AFalseKnight();
	~AFalseKnight();

	// delete Function
	AFalseKnight(const AFalseKnight& _Other) = delete;
	AFalseKnight(AFalseKnight&& _Other) noexcept = delete;
	AFalseKnight& operator=(const AFalseKnight& _Other) = delete;
	AFalseKnight& operator=(AFalseKnight&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;
	void SetStatus() override;
	void SetOffset() override;
	void CreateAnimation() override;

private:
	void BossPatternTimeElapsed(float _DeltaTime);
	float JumpElapsed = 0.0f;
	float JumpAttackElapsed = 0.0f;

	float JumpActionInitElapsed = 0.0f;
	bool bCanJumpAttack = false;
	
	bool bIsResting = false;
	float RestElapsed = 0.0f;


	void DamageEffect() override;

	bool bCanFlashEffect = false;
	bool bIsEndFlashEffect = false;
	void StartFlashEffect(float _DeltaTime);
	void EndFlashEffect(float _DeltaTime);

	void CreateAttackLogicAndEffect();
	void CreateGroundImpack();
	bool bIsOffsetAttack1Frame = false;
	bool bISOffsetAttackRecovery1Fame = false;

	void CreateJumpAttackLogicAndEffect();

	bool bIsBerserkMode = false; // 첫 스턴 전까진 광폭 상태가 아니다.

private:
	void SetIdle(float _DeltaTime) override;
	void SetJumpAnticipate(float _DeltaTime) override;
	void SetJump(float _DeltaTime) override;
	void SetLand(float _DeltaTime);

	void SetJumpAttackAnticipate(float _DeltaTime) override;
	void SetJumpAttack(float _DeltaTime) override;
	void SetJumpAttackRecovery(float _DeltaTime) override;
	void SetJumpAttackLand(float _DeltaTime) override;

	void SetAttackAnticipate(float _DeltaTime) override;
	void SetAttack(float _DeltaTime) override;
	void SetAttackRecovery(float _DeltaTime) override;
	void SetAttackRecovery2(float _DeltaTime) override;

	void SetBerserkAttackAnticipate(float _DeltaTime) override;
	void SetBerserkAttack(float _DeltaTime) override;
	void SetBerserkAttack2(float _DeltaTime) override;
	void SetBerserkAttackRecovery(float _DeltaTime) override;
};

