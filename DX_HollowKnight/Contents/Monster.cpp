#include "PreCompile.h"
#include "Monster.h"

AMonster::AMonster()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;


	// Renderer
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = static_cast<float>(EZOrder::MONSTER);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	CreateAnimation();

	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D({150, 150});
	BodyCollision->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	//BodyCollision->SetActive(false);
	BodyCollision->SetCollisionProfileName("Monster");


}

AMonster::~AMonster()
{
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	CreateCollision();
	SetFSM();
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	FSM.Update(_DeltaTime);

}

void AMonster::Move(float _DeltaTime)
{
}

void AMonster::TimeElapsed(float _DeltaTime)
{
}

void AMonster::DebugInput(float _DeltaTime)
{
}

void AMonster::CreateAnimation()
{
	std::string HuskBully = "HuskBully";
	float FrameTime = 0.2f;
	BodyRenderer->CreateAnimation("Idle", HuskBully, 0, 5, FrameTime);
	BodyRenderer->CreateAnimation("Walk", HuskBully, 6, 12, FrameTime);
	BodyRenderer->CreateAnimation("Turn", HuskBully, 13, 14, FrameTime);
	BodyRenderer->CreateAnimation("Attack_Anticipate", HuskBully, 15, 18, FrameTime);
	BodyRenderer->CreateAnimation("Attack_Lunge", HuskBully, 19, 20, FrameTime);
	BodyRenderer->CreateAnimation("Attack_Cooldown", HuskBully, 21, 21, FrameTime);
	BodyRenderer->CreateAnimation("Death_Air", HuskBully, 22, 22, FrameTime);
	BodyRenderer->CreateAnimation("Death_Land", HuskBully, 23, 30, FrameTime);



	BodyRenderer->ChangeAnimation("Idle");


	std::string Vengefly = "Vengefly";
	BodyRenderer->CreateAnimation(Vengefly, Vengefly, 0, 2, 0.2f);





	
}

void AMonster::CreateCollision()
{
	BodyCollision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			//_Other->GetActor()->Destroy();
			// _Other->Destroy();
			//UEngineDebug::OutPutString("Enter");
		});
}

bool AMonster::CanAction()
{
	return false;
}

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
	CreateState(EMonsterState::LAND, &AMonster::SetLand	, "Land");
	CreateState(EMonsterState::HARD_LAND, &AMonster::SetHardLand, "HardLand");

	CreateState(EMonsterState::DASH_ARRIVE, &AMonster::SetDashArrive, "DashArrive");
	CreateState(EMonsterState::DASH_ANTICIPATE, &AMonster::SetDashAnticipate, "DashAnticipate");
	CreateState(EMonsterState::DASH, &AMonster::SetDash, "Dash");
	CreateState(EMonsterState::DASH_ATTACK, &AMonster::SetDashAttack, "DashAttack");
	CreateState(EMonsterState::DASH_RECOVERY, &AMonster::SetDashRecovery, "DashRecovery");

	CreateState(EMonsterState::ATTACK_ANTICIPATE, &AMonster::SetAttackAnticipate, "AttackAnticipate");
	CreateState(EMonsterState::ATTACK, &AMonster::SetAttack, "Attack");
	CreateState(EMonsterState::ATTACK_RECOVERY, &AMonster::SetAttackRecovery, "AttackRecovery");

	CreateState(EMonsterState::THROW_ANTICIPATE, &AMonster::SetThrowAnticipate, "ThrowAnticipate");
	CreateState(EMonsterState::THROW, &AMonster::SetThrow, "Throw");
	CreateState(EMonsterState::THROW_RECOVERY, &AMonster::SetThrowRecovery, "ThrowRecovery");

	CreateState(EMonsterState::WALL_ANTICIPATE, &AMonster::SetWallAnticipate, "WallAnticipate");
	CreateState(EMonsterState::WALK, &AMonster::SetWall, "Wall");
	CreateState(EMonsterState::WALL_ATTACK, &AMonster::SetWallAttack, "WallAttack");
	CreateState(EMonsterState::WALL_RECOVERY, &AMonster::SetWallRecovery, "WallRecovery");

	CreateState(EMonsterState::COUNTER_ANTICIPATE, &AMonster::SetCounterAnticipate, "CounterAnticipate");
	CreateState(EMonsterState::COUNTER_STANCE, &AMonster::SetCounterStance, "CounterStance");
	CreateState(EMonsterState::COUNTER, &AMonster::SetCounter, "Counter");
	CreateState(EMonsterState::COUNTER_RECOVERY, &AMonster::SetCounterRecovery, "CounterRecovery");
	CreateState(EMonsterState::COUNTER_ATTACK, &AMonster::SetCounterAttack, "CounterAttack");
	CreateState(EMonsterState::COUNTER_ATTACK_RECOVERY, &AMonster::SetCounterAttackRecovery, "CounterAttackRecovery");
	CreateState(EMonsterState::COUNTER_ATTACK_RECOVERY, &AMonster::SetCounterAttackRecovery, "CounterAttackRecovery");
	CreateState(EMonsterState::SHIELD_ANTICIPATE, &AMonster::SetShieldAnticipate, "ShieldAnticipate");
	CreateState(EMonsterState::SHIELD_FRONT, &AMonster::SetShieldFront, "ShieldFront");
	CreateState(EMonsterState::SHIELD_FRONT_BUMP, &AMonster::SetShieldFrontBump, "ShieldFrontBump");
	CreateState(EMonsterState::SHIELD_TOP, &AMonster::SetShieldTop, "ShieldTop");
	CreateState(EMonsterState::SHIELD_TOP_BUMP, &AMonster::SetShieldTopBump, "ShieldTopBump");

	CreateState(EMonsterState::EVADE_ANTICIPATE, &AMonster::SetEvadeAnticipate, "EvadeAnticipate");
	CreateState(EMonsterState::EVADE, &AMonster::SetEvade, "Evade");

	CreateState(EMonsterState::STUN, &AMonster::SetStun, "Stun");
	CreateState(EMonsterState::STUN_ROLL, &AMonster::SetStunRoll, "StunRoll");
	CreateState(EMonsterState::STUN_ROLL_END, &AMonster::SetStunRollEnd, "StunRollEnd");
	CreateState(EMonsterState::STUN_OPEN, &AMonster::SetStunOpen, "StunOpen");
	CreateState(EMonsterState::STUN_HIT, &AMonster::SetStunHit, "StunHit");
	CreateState(EMonsterState::STUN_HIT, &AMonster::SetStunHit, "StunHit");

	CreateState(EMonsterState::DEATH_AIR, &AMonster::SetDeathAir, "DeathAir");
	CreateState(EMonsterState::DEATH_LAND, &AMonster::SetDeathLand, "DeathLand");

	CreateState(EMonsterState::FLY, &AMonster::SetFly, "Fly");
	CreateState(EMonsterState::FIRE, &AMonster::SetFire, "Fire");
	CreateState(EMonsterState::BURST, &AMonster::SetBurst, "Burst");
}

void AMonster::SetIdle(float _DeltaTime)
{
}

void AMonster::SetWalk(float _DeltaTime)
{
}

void AMonster::SetRun(float _DeltaTime)
{
}

void AMonster::SetTurn(float _DeltaTime)
{
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

void AMonster::SetAttackAnticipate(float _DeltaTime)
{
}

void AMonster::SetAttack(float _DeltaTime)
{
}

void AMonster::SetAttackRecovery(float _DeltaTime)
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

void AMonster::SetDeathAir(float _DeltaTime)
{
}

void AMonster::SetDeathLand(float _DeltaTime)
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

void AMonster::CreateState(EMonsterState _State, StateCallback _Callback, std::string_view _AnimationName)
{
	FSM.CreateState(_State, std::bind(_Callback, this, std::placeholders::_1),
		[this, _AnimationName]()
		{
			std::string AnimationName = _AnimationName.data();
			BodyRenderer->ChangeAnimation(AnimationName);
		});
}

void AMonster::CheckDirection()
{
	if (false == CanAction())
	{
		return;
	}
	if (false == bCanRotation)
	{
		return;
	}
	if (bIsLeft = true)
	{
		SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
	}
	if (bIsLeft = false)
	{
		SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
	}
}

