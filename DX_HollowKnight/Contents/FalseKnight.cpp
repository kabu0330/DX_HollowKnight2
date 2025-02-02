#include "PreCompile.h"
#include "FalseKnight.h"

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
	Data.Velocity = 300.0f;
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
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = true; 
	bIsAggressive = true; // ȣ�����̸� true
	bCanFly = false; // ���ƴٴϴ� ���Ͷ�� true

	MoveDuration = 2.0f;
	MoveCooldown = 3.0f;

	AttackFrame = 0.15f;// �Ź� ����
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;

	bIsDeathDestroy = false; // ������ ��ü ���� �Ҹ��Ұ���

}

void AFalseKnight::SetOffset()
{
	//FVector ImageSize = { 479.0f , 469.0f };
	FVector SpriteSize = { 290.0f, 300.0f };

	bIsFlip = true; // ������ �¿����

	BodyCollisionOffset = { 60.0f, 0.0f }; // �ø��� ��ġ�� ���� �ȼ��浹 ��ġ�� �ٲ��.
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
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.1f; // ���ư��� �ð� ������ ���� �ڿ��������� ǥ���ȴ�.
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	//BodyRenderer->CreateAnimation("WalkAnticipate", MonsterStr, 7, 8, RunnigTime, false);
	//BodyRenderer->CreateAnimation("Walk", MonsterStr, 9, 13, RunnigTime);
	BodyRenderer->CreateAnimation("JumpAnticipate", MonsterStr, 13, 15, RunnigTime, false);
	BodyRenderer->CreateAnimation("Jump", MonsterStr, 16, 19, JumpTime, false);
	BodyRenderer->CreateAnimation("Jumping", MonsterStr, 19, 19, JumpTime, false);
	BodyRenderer->CreateAnimation("Land", MonsterStr, 20, 22, JumpTime, false);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 24, 29, AttackAnticipateTime, false);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 30, 32, AttackFrame, false);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 33, 37, AttackRecoveryFrame, false);
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
	if (false == bCanJump)
	{
		JumpElapsed += _DeltaTime;
		float Cooldown = 3.0f;
		if (JumpElapsed >= Cooldown)
		{
			JumpElapsed = 0.0f;
			bCanJump = true;
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

	bCanRotation = true; // ������ȯ ���
	CheckDirection(); // �¿� ���� ����

	GetRandomDirection(); // chasing�� false��� �����̵�
	GetDirectionToPlayer(); // chasing�� true��� ����

	if (false == bIsOnGround && false == bCanFly)
	{
		return;
	}
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanAttack) // �÷��̾� ����
	{
		//FSM.ChangeState(EMonsterState::ATTACK_ANTICIPATE);
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

	BodyRenderer->SetRelativeLocation({ 0.0f, 0.0f });
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

