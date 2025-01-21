#include "PreCompile.h"
#include "Monster.h"

void AMonster::SetFSM()
{
	CreateState(EMonsterState::IDLE, &AMonster::SetIdle, "Idle");
	CreateState(EMonsterState::WALK, &AMonster::SetWalk, "Walk");
	CreateState(EMonsterState::RUN, &AMonster::SetRun, "Run");
	CreateState(EMonsterState::TURN, &AMonster::SetTurn, "Turn");

	CreateState(EMonsterState::JUMP_ANTICIPATE, &AMonster::SetJumpAnticipate, "JumpAnticipate");
	CreateState(EMonsterState::JUMP, &AMonster::SetJump, "Jump");
	CreateState(EMonsterState::JUMP_ATTACK_ANTICIPATE, &AMonster::SetJumpAttackAnticipate, "JumpAttackAnticipate");
	CreateState(EMonsterState::JUMP_ATTACK, &AMonster::SetJumpAttack, "JumpAttack");
	CreateState(EMonsterState::LAND, &AMonster::SetLand, "Land");
	CreateState(EMonsterState::HARD_LAND, &AMonster::SetHardLand, "HardLand");

	// 공격스킬에 해당
	CreateState(EMonsterState::DASH_ARRIVE, &AMonster::SetDashArrive, "DashArrive");
	CreateState(EMonsterState::DASH_ANTICIPATE, &AMonster::SetDashAnticipate, "DashAnticipate");
	CreateState(EMonsterState::DASH, &AMonster::SetDash, "Dash");
	CreateState(EMonsterState::DASH_ATTACK, &AMonster::SetDashAttack, "DashAttack");
	CreateState(EMonsterState::DASH_RECOVERY, &AMonster::SetDashRecovery, "DashRecovery");

	// 공격
	CreateState(EMonsterState::ATTACK_ANTICIPATE, &AMonster::SetAttackAnticipate, "AttackAnticipate");
	CreateState(EMonsterState::ATTACK, &AMonster::SetAttack, "Attack");
	CreateState(EMonsterState::ATTACK_RECOVERY, &AMonster::SetAttackRecovery, "AttackRecovery");

	CreateState(EMonsterState::THROW_ANTICIPATE, &AMonster::SetThrowAnticipate, "ThrowAnticipate");
	CreateState(EMonsterState::THROW, &AMonster::SetThrow, "Throw");
	CreateState(EMonsterState::THROW_RECOVERY, &AMonster::SetThrowRecovery, "ThrowRecovery");

	CreateState(EMonsterState::WALL_ANTICIPATE, &AMonster::SetWallAnticipate, "WallAnticipate");
	CreateState(EMonsterState::WALL, &AMonster::SetWall, "Wall");
	CreateState(EMonsterState::WALL_ATTACK, &AMonster::SetWallAttack, "WallAttack");
	CreateState(EMonsterState::WALL_RECOVERY, &AMonster::SetWallRecovery, "WallRecovery");

	CreateState(EMonsterState::COUNTER_ANTICIPATE, &AMonster::SetCounterAnticipate, "CounterAnticipate");
	CreateState(EMonsterState::COUNTER_STANCE, &AMonster::SetCounterStance, "CounterStance");
	CreateState(EMonsterState::COUNTER, &AMonster::SetCounter, "Counter");
	CreateState(EMonsterState::COUNTER_RECOVERY, &AMonster::SetCounterRecovery, "CounterRecovery");
	CreateState(EMonsterState::COUNTER_ATTACK, &AMonster::SetCounterAttack, "CounterAttack");
	CreateState(EMonsterState::COUNTER_ATTACK_RECOVERY, &AMonster::SetCounterAttackRecovery, "CounterAttackRecovery");
	CreateState(EMonsterState::SHIELD_ANTICIPATE, &AMonster::SetShieldAnticipate, "ShieldAnticipate");
	CreateState(EMonsterState::SHIELD_FRONT, &AMonster::SetShieldFront, "ShieldFront");
	CreateState(EMonsterState::SHIELD_FRONT_BUMP, &AMonster::SetShieldFrontBump, "ShieldFrontBump");
	CreateState(EMonsterState::SHIELD_TOP, &AMonster::SetShieldTop, "ShieldTop");
	CreateState(EMonsterState::SHIELD_TOP_BUMP, &AMonster::SetShieldTopBump, "ShieldTopBump");

	CreateState(EMonsterState::EVADE_ANTICIPATE, &AMonster::SetEvadeAnticipate, "EvadeAnticipate");
	CreateState(EMonsterState::EVADE, &AMonster::SetEvade, "Evade");

	CreateState(EMonsterState::HIT, &AMonster::SetHit, "Hit");
	CreateState(EMonsterState::STUN, &AMonster::SetStun, "Stun");
	CreateState(EMonsterState::STUN_ROLL, &AMonster::SetStunRoll, "StunRoll");
	CreateState(EMonsterState::STUN_ROLL_END, &AMonster::SetStunRollEnd, "StunRollEnd");
	CreateState(EMonsterState::STUN_OPEN, &AMonster::SetStunOpen, "StunOpen");
	CreateState(EMonsterState::STUN_HIT, &AMonster::SetStunHit, "StunHit");

	CreateState(EMonsterState::DEATH_AIR, &AMonster::SetDeathAir, "DeathAir");
	CreateState(EMonsterState::DEATH_LAND, &AMonster::SetDeathLand, "DeathLand");

	CreateState(EMonsterState::FLY, &AMonster::SetFly, "Fly");
	CreateState(EMonsterState::FIRE, &AMonster::SetFire, "Fire");
	CreateState(EMonsterState::BURST, &AMonster::SetBurst, "Burst");

	FSM.ChangeState(EMonsterState::IDLE);
}

void AMonster::SetIdle(float _DeltaTime)
{
	UEngineDebug::OutPutString("FSM : Idle");

	ActiveGravity();
	CheckDeath();

	Stat.SetBeingHit(false);
	Stat.SetAttacking(false);

	bChooseDirection = false; // 이동할 방향 결정 허용
	bCanRotation = true; // 방향전환 허용
	CheckDirection(); // 좌우 반전 적용



	if (false == bIsOnGround)
	{
		return;
	}
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanAttack) // 플레이어 조우
	{
		FSM.ChangeState(EMonsterState::ATTACK_ANTICIPATE);
	}
	else if (true == IsPlayerNearby() || true == bCanMove)
	{
		FSM.ChangeState(EMonsterState::TURN);
	}
}

void AMonster::SetWalk(float _DeltaTime)
{
	UEngineDebug::OutPutString("FSM : Walk");

	CheckDeath();
	ActiveGravity();

	if (false == bIsOnGround)
	{
		return;
	}

	if (false == bIsChasing) // 추적중
	{
		GetRandomDirection(); // 랜덤 이동 
	}
	else
	{
		GetDirectionToPlayer();
	}

	CheckDirection(); // 좌우 반전 적용
	Move(_DeltaTime); // 이동 및 방향전환 금지
	bIsTurn = false;

	if (true == Stat.IsBeingHit())
	{
		FSM.ChangeState(EMonsterState::IDLE);
	}
	else if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanAttack) // 플레이어 조우
	{
		FSM.ChangeState(EMonsterState::ATTACK_ANTICIPATE);
	}
	else if (false == bCanMove)
	{
		FSM.ChangeState(EMonsterState::IDLE);
	}
}

void AMonster::SetRun(float _DeltaTime)
{
	ActiveGravity();
}

void AMonster::SetTurn(float _DeltaTime)
{
	CheckDeath();
	ActiveGravity();

	CheckDirection(); // 좌우 반전 적용
	if (true == bIsTurn)
	{
		ChangeNextState(EMonsterState::WALK);
	}
	else
	{
		FSM.ChangeState(EMonsterState::WALK);
	}

}

void AMonster::SetAttackAnticipate(float _DeltaTime)
{
	CheckDeath();
	ActiveGravity();

	UEngineDebug::OutPutString("몬스터 공격 준비");

	Stat.SetAttacking(true);
	bCanAttack = false;
	bCanRotation = false;
	ChangeNextState(EMonsterState::ATTACK);
}

void AMonster::SetAttack(float _DeltaTime)
{
	CheckDeath();
	ActiveGravity();

	UEngineDebug::OutPutString("몬스터 공격~~~~~~~~~");
	Dash();

	if (true == Stat.IsBeingHit())
	{
		FSM.ChangeState(EMonsterState::IDLE);
	}
	else
	{
		TimeEventor->AddEndEvent(0.3f, [this]()
			{
				FSM.ChangeState(EMonsterState::ATTACK_RECOVERY);
			});
	}
}

void AMonster::SetAttackRecovery(float _DeltaTime)
{
	CheckDeath();
	ActiveGravity();

	UEngineDebug::OutPutString("FSM : Attack Recovery");
	ChangeNextState(EMonsterState::IDLE);
}

void AMonster::SetHit(float _DeltaTime)
{
	CheckDeath();
	ActiveGravity();
}

void AMonster::SetDeathAir(float _DeltaTime)
{
	ActiveGravity();
	BodyCollision->Destroy();
	ChangeNextState(EMonsterState::DEATH_LAND);
}

void AMonster::SetDeathLand(float _DeltaTime)
{
	ActiveGravity();

	Stat.SetDeath(true);
}

void AMonster::SetJumpAnticipate(float _DeltaTime)
{
}

void AMonster::SetJump(float _DeltaTime)
{
}

void AMonster::SetJumpAttackAnticipate(float _DeltaTime)
{
}

void AMonster::SetJumpAttack(float _DeltaTime)
{
}

void AMonster::SetLand(float _DeltaTime)
{
}

void AMonster::SetHardLand(float _DeltaTime)
{
}

void AMonster::SetDashArrive(float _DeltaTime)
{
}

void AMonster::SetDashAnticipate(float _DeltaTime)
{
}

void AMonster::SetDash(float _DeltaTime)
{
}

void AMonster::SetDashAttack(float _DeltaTime)
{
}

void AMonster::SetDashRecovery(float _DeltaTime)
{
}

void AMonster::SetThrowAnticipate(float _DeltaTime)
{
}

void AMonster::SetThrow(float _DeltaTime)
{
}

void AMonster::SetThrowRecovery(float _DeltaTime)
{
}

void AMonster::SetWallAnticipate(float _DeltaTime)
{
}

void AMonster::SetWall(float _DeltaTime)
{
}

void AMonster::SetWallAttack(float _DeltaTime)
{
}

void AMonster::SetWallRecovery(float _DeltaTime)
{
}

void AMonster::SetCounterAnticipate(float _DeltaTime)
{
}

void AMonster::SetCounterStance(float _DeltaTime)
{
}

void AMonster::SetCounter(float _DeltaTime)
{
}

void AMonster::SetCounterAttack(float _DeltaTime)
{
}

void AMonster::SetCounterAttackRecovery(float _DeltaTime)
{
}

void AMonster::SetCounterRecovery(float _DeltaTime)
{
}

void AMonster::SetShieldAnticipate(float _DeltaTime)
{
}

void AMonster::SetShieldFront(float _DeltaTime)
{
}

void AMonster::SetShieldFrontBump(float _DeltaTime)
{
}

void AMonster::SetShieldTop(float _DeltaTime)
{
}

void AMonster::SetShieldTopBump(float _DeltaTime)
{
}

void AMonster::SetEvadeAnticipate(float _DeltaTime)
{
}

void AMonster::SetEvade(float _DeltaTime)
{
}

void AMonster::SetStun(float _DeltaTime)
{
}

void AMonster::SetStunRoll(float _DeltaTime)
{
}

void AMonster::SetStunRollEnd(float _DeltaTime)
{
}

void AMonster::SetStunOpen(float _DeltaTime)
{
}

void AMonster::SetStunHit(float _DeltaTime)
{
}

void AMonster::SetFly(float _DeltaTime)
{
}

void AMonster::SetFire(float _DeltaTime)
{
}

void AMonster::SetBurst(float _DeltaTime)
{
}

void AMonster::ChangeNextState(EMonsterState _NextState)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(_NextState);
		return;
	}
}

void AMonster::ChangePrevState()
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangePrevState();
		return;
	}
}

void AMonster::CreateState(EMonsterState _State, StateCallback _Callback, std::string_view _AnimationName)
{
	FSM.CreateState(_State, std::bind(_Callback, this, std::placeholders::_1),
		[this, _AnimationName]()
		{
			std::string AnimationName = _AnimationName.data();
			BodyRenderer->ChangeAnimation(AnimationName);
		});
}