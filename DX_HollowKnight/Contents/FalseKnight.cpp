#include "PreCompile.h"
#include "FalseKnight.h"
#include "MonsterEffect.h"

AFalseKnight::AFalseKnight()
{
}

AFalseKnight::~AFalseKnight()
{
}

void AFalseKnight::BeginPlay()
{
	AMonster::BeginPlay();
}

void AFalseKnight::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
	BossPatternTimeElapsed(_DeltaTime);
	StartFlashEffect(_DeltaTime);
	EndFlashEffect(_DeltaTime);
}

void AFalseKnight::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 0.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 385;
	Data.Hp = 300;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = false;
	Data.KnockbackDistance = 0.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 15.0f;
	JumpForceMax = 1000.0f;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = true; 
	bIsAggressive = true; // 호전적이면 true
	bCanFly = false; // 날아다니는 몬스터라면 true

	MoveDuration = 2.0f;
	MoveCooldown = 3.0f;

	AttackFrame = 0.15f;// 매번 설정
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 7.0f;

	AttackRecoveryFrame = 0.2f;

	bIsDeathDestroy = false; // 죽으면 시체 없이 소멸할건지

}

void AFalseKnight::SetOffset()
{
	//FVector ImageSize = { 479.0f , 469.0f };
	FVector SpriteSize = { 300.0f, 280.0f };

	bIsFlip = true; // 렌더러 좌우반전

	BodyCollisionOffset = { 60.0f, -10.0f }; // 컬리전 위치에 따라 픽셀충돌 위치도 바뀐다.
	RendererOffset = { 0.0f, 40.0f };

	CollisionScale = SpriteSize;
	DetectRange = { 2000, 500 };

	DeathSpriteOffset = 0.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
}

void AFalseKnight::CreateAnimation()
{
	SetName("FalseKnight");
	std::string MonsterStr = "FalseKnight.png";
	float IdleTime = 0.2f;
	float JumpTime = 0.2f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.2f;
	float DeathAirTime = 0.1f; // 날아가는 시간 조절에 따라서 자연스러움이 표현된다.
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	//BodyRenderer->CreateAnimation("WalkAnticipate", MonsterStr, 7, 8, RunnigTime, false);
	//BodyRenderer->CreateAnimation("Walk", MonsterStr, 9, 13, RunnigTime);
	BodyRenderer->CreateAnimation("JumpAnticipate", MonsterStr, 13, 15, RunnigTime, false);
	BodyRenderer->CreateAnimation("Jump", MonsterStr, 16, 19, JumpTime, false);
	BodyRenderer->CreateAnimation("Jumping", MonsterStr, 19, 19, JumpTime, false);
	BodyRenderer->CreateAnimation("Land", MonsterStr, 20, 22, JumpTime, false);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 23, 28, AttackAnticipateTime, false);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 29, 31, AttackFrame, false);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 32, 34, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("AttackRecovery2", MonsterStr, 35, 36, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("JumpAttackAnticipate", MonsterStr, 38, 42, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("JumpAttack", MonsterStr, 43, 44, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("JumpAttackRecovery", MonsterStr, 45, 48, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("Stun", MonsterStr, 49, 62, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 78, 80, DeathAirTime, false);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 81, 86, DeathTime, false);

	BodyRenderer->ChangeAnimation("Idle");
}

void AFalseKnight::BossPatternTimeElapsed(float _DeltaTime)
{
	// 점프 이동
	if (false == bCanJump)
	{
		JumpElapsed += _DeltaTime;
		float Cooldown = 7.0f;
		if (JumpElapsed >= Cooldown)
		{
			JumpElapsed = 0.0f;
			bCanJump = true;
		}
	}

	// 공격 이후 즉시 다음 공격을 하지 않도록
	if (true == bIsResting)
	{
		RestElapsed += _DeltaTime;
		float Cooldown = 3.0f;
		if (RestElapsed >= Cooldown)
		{
			RestElapsed = 0.0f;
			bIsResting = false;
		}
	}
}

void AFalseKnight::DamageEffect()
{
	bCanFlashEffect = true;
}

void AFalseKnight::StartFlashEffect(float _DeltaTime)
{
	if (false == bCanFlashEffect)
	{
		return;
	}
	if (false == BodyRenderer->IsActive())
	{
		return;
	}
	if (true == bIsEndFlashEffect)
	{
		return;
	}
	
	FVector PlusColor = { 1.0f, 1.0f, 1.0f };
	float4 MaxColor = { 1.0f, 1.0f, 1.0f};

	if (MaxColor.X <= BodyRenderer->ColorData.PlusColor.X)
	{
		bIsEndFlashEffect = true;
		return;
	}
	float Power = 7.0f;
	PlusColor *= _DeltaTime * Power;
	BodyRenderer->AddPlusColor(PlusColor);

}

void AFalseKnight::EndFlashEffect(float _DeltaTime)
{
	if (false == bCanFlashEffect)
	{
		return;
	}
	if (false == bIsEndFlashEffect)
	{
		return;
	}

	if (0.0f >= BodyRenderer->ColorData.PlusColor.X)
	{
		//BodyRenderer->ColorData.PlusColor = { 0.0f, 0.0f, 0.0f };
		bCanFlashEffect = false;
		bIsEndFlashEffect = false;
		return;
	}

	FVector PlusColor = { 1.0f, 1.0f, 1.0f };
	float4 MaxColor = { 1.0f, 1.0f, 1.0f };

	float Power = 7.0f;
	PlusColor *= -_DeltaTime * Power;
	BodyRenderer->AddPlusColor(PlusColor);

	float4 Data = BodyRenderer->ColorData.PlusColor;
}

void AFalseKnight::SetIdle(float _DeltaTime)
{
	ResetRendererOffest();

	ActiveGravity();
	CheckDeath();

	Stat.SetBeingHit(false);
	Stat.SetAttacking(false);

	bCanRotation = true; // 방향전환 허용
	CheckDirection(); // 좌우 반전 적용

	GetRandomDirection(); // chasing이 false라면 랜덤이동
	GetDirectionToPlayer(); // chasing이 true라면 추적

	if (false == bIsOnGround && false == bCanFly)
	{
		return;
	}
	if (true == bIsResting) // 직전에 이미 기술을 사용했으면 잠시 Idle 상태 고정
	{
		return;
	}
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanAttack) // 플레이어 조우
	{
		FSM.ChangeState(EMonsterState::ATTACK_ANTICIPATE);
	}
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanJump) 
	{
		FSM.ChangeState(EMonsterState::JUMP_ANTICIPATE);
	}

}

void AFalseKnight::SetJumpAnticipate(float _DeltaTime)
{
	ActiveGravity();
	bCanJump = false;

	ChangeNextState(EMonsterState::JUMP);
}

void AFalseKnight::SetJump(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	JumpActionInitElapsed += _DeltaTime;
	float JumpActionTime = 1.0f;
	if (JumpActionInitElapsed <= JumpActionTime)
	{
		Jump(_DeltaTime);
	}
	else if (JumpActionInitElapsed >= JumpActionTime && true == bIsOnGround)
	{
		JumpActionInitElapsed = 0.0f;
		FSM.ChangeState(EMonsterState::LAND);
	}
}


void AFalseKnight::SetLand(float _DeltaTime)
{
	ActiveGravity();

	JumpForce = 0.0f;
	bIsResting = true; // 패턴과 패턴 간 쿨타임 적용

	ChangeNextState(EMonsterState::IDLE);
}

void AFalseKnight::SetJumpAttackAnticipate(float _DeltaTime)
{
}

void AFalseKnight::SetJumpAttack(float _DeltaTime)
{
}

void AFalseKnight::SetJumpAttackRecovery(float _DeltaTime)
{
}

void AFalseKnight::SetAttackAnticipate(float _DeltaTime)
{
	CheckDeath();
	ActiveGravity();

	// 렌더러 위치 조정
	FVector Offset = { -35.0f, 20.0f };
	BodyRenderer->SetRelativeLocation(Offset); 
	
	Stat.SetAttacking(true);
	bCanAttack = false;
	bCanRotation = false;
	ChangeNextState(EMonsterState::ATTACK);
}

void AFalseKnight::SetAttack(float _DeltaTime)
{
	ActiveGravity();

	// 렌더러 위치 조정
	if (false == bIsOffsetAttack1Frame)
	{
		FVector Offset = { 150.0f, 130.0f };
		BodyRenderer->SetRelativeLocation(Offset);
		bIsOffsetAttack1Frame = true;
	}
	if (false == bIsShowEffect) // 한 번만 공격 로직 호출
	{
		TimeEventer->AddEndEvent(0.15f, [this]()
			{
				FVector Offset = { 150.0f, 67.0f };
				BodyRenderer->SetRelativeLocation(Offset);
				CreateAttackLogicAndEffect();
			});
	}
	bIsShowEffect = true;

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			FVector Offset = { 150.0f, 0.0f };
			BodyRenderer->SetRelativeLocation(Offset);
		});

	ChangeNextState(EMonsterState::ATTACK_RECOVERY);
}

// 공격 컬리전 생성
void AFalseKnight::CreateAttackLogicAndEffect()
{
	// 컬리전 생성
	std::shared_ptr<AMonsterSkill> Skill = GetWorld()->SpawnActor<AMonsterSkill>();

	//Skill->SetCollisionTime(AttackDuration);
	Skill->SetCollisionTime(0.1f);

	FVector CollisionScale = FVector(150, 150);
	Skill->SetCollisionScale(CollisionScale);

	// 이펙트 생성
	AMonsterEffect* Effect = GetWorld()->SpawnActor<AMonsterEffect>().get();
	FVector Pos = GetActorLocation();
	Effect->ChangeAnimation("GroundImapctEffect", { Pos.X, Pos.Y });
	Effect->SetScale(1.5f);

	// 그라운드 웨이브(이동하는 충격파) 생성
	std::shared_ptr<AMonsterSkill> GroundWave = GetWorld()->SpawnActor<AMonsterSkill>();
	GroundWave->ChangeAnimation("BossGroundWave");
	GroundWave->ChangeNextAnimation("BossGroundWaveLoop"); // 현재 애니메이션이 끝나면 바꿀 애니메이션
	GroundWave->SetAutoRelease(false);
	GroundWave->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT) - 1);
	GroundWave->SetParentRoom(ParentRoom); // 픽셀충돌 검사
	GroundWave->SetCollisionTime(AttackDuration * 5.0f);
	GroundWave->SetCollisionScale({ 180.0f, 50.0f });
	GroundWave->SetScale(1.3f);
	

	FVector Offset = FVector{ -330.0f, -100.0f };
	FVector EffectOffset = FVector(0.0f, 20.0f);
	FVector GroundWaveOffset = FVector(-330.0f, -70.0f);
	FVector Speed = FVector(900.0f, 0.0f);
	if (true == bIsLeft)
	{
		Skill->SetLocation(this, { Offset.X, Offset.Y });
		Effect->SetLocation(this, { Offset.X, Offset.Y + EffectOffset.Y});

		GroundWave->AddLocation(this, -Speed, GroundWaveOffset);
	}
	else
	{
		Skill->SetLocation(this, { Offset.X, -Offset.Y });
		Effect->SetLocation(this, { Offset.X, -(Offset.Y + EffectOffset.Y) });
		GroundWave->AddLocation(this, Speed, { GroundWaveOffset.X, -GroundWaveOffset.Y });
	}
		
}

void AFalseKnight::SetAttackRecovery(float _DeltaTime)
{
	ActiveGravity();

	ResetRendererOffest();
	FVector Offset = { 0.0f, 120.0f };
	BodyRenderer->SetRelativeLocation(Offset);

	bIsShowEffect = false;
	bIsOffsetAttack1Frame = false;
	bIsResting = true; // 패턴과 패턴 간 쿨타임 적용

	ChangeNextState(EMonsterState::ATTACK_RECOVERY2);
}

void AFalseKnight::SetAttackRecovery2(float _DeltaTime)
{
	ActiveGravity();

	ResetRendererOffest();

	ChangeNextState(EMonsterState::IDLE);
}

