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
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = true; 
	bIsAggressive = true; // ȣ�����̸� true
	bCanFly = false; // ���ƴٴϴ� ���Ͷ�� true

	MoveDuration = 2.0f;
	MoveCooldown = 3.0f;

	AttackFrame = 0.15f;// �Ź� ����
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 7.0f;

	AttackRecoveryFrame = 0.2f;

	bIsDeathDestroy = false; // ������ ��ü ���� �Ҹ��Ұ���

}

void AFalseKnight::SetOffset()
{
	//FVector ImageSize = { 479.0f , 469.0f };
	FVector SpriteSize = { 300.0f, 280.0f };

	bIsFlip = true; // ������ �¿����

	BodyCollisionOffset = { 60.0f, -10.0f }; // �ø��� ��ġ�� ���� �ȼ��浹 ��ġ�� �ٲ��.
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
	float DeathAirTime = 0.1f; // ���ư��� �ð� ������ ���� �ڿ��������� ǥ���ȴ�.
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
	// ���� �̵�
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

	// ���� ���� ��� ���� ������ ���� �ʵ���
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

	bCanRotation = true; // ������ȯ ���
	CheckDirection(); // �¿� ���� ����

	GetRandomDirection(); // chasing�� false��� �����̵�
	GetDirectionToPlayer(); // chasing�� true��� ����

	if (false == bIsOnGround && false == bCanFly)
	{
		return;
	}
	if (true == bIsResting) // ������ �̹� ����� ��������� ��� Idle ���� ����
	{
		return;
	}
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanAttack) // �÷��̾� ����
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
	bIsResting = true; // ���ϰ� ���� �� ��Ÿ�� ����

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

	// ������ ��ġ ����
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

	// ������ ��ġ ����
	if (false == bIsOffsetAttack1Frame)
	{
		FVector Offset = { 150.0f, 130.0f };
		BodyRenderer->SetRelativeLocation(Offset);
		bIsOffsetAttack1Frame = true;
	}
	if (false == bIsShowEffect) // �� ���� ���� ���� ȣ��
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

// ���� �ø��� ����
void AFalseKnight::CreateAttackLogicAndEffect()
{
	// �ø��� ����
	std::shared_ptr<AMonsterSkill> Skill = GetWorld()->SpawnActor<AMonsterSkill>();

	//Skill->SetCollisionTime(AttackDuration);
	Skill->SetCollisionTime(0.1f);

	FVector CollisionScale = FVector(150, 150);
	Skill->SetCollisionScale(CollisionScale);

	// ����Ʈ ����
	AMonsterEffect* Effect = GetWorld()->SpawnActor<AMonsterEffect>().get();
	FVector Pos = GetActorLocation();
	Effect->ChangeAnimation("GroundImapctEffect", { Pos.X, Pos.Y });
	Effect->SetScale(1.5f);

	// �׶��� ���̺�(�̵��ϴ� �����) ����
	std::shared_ptr<AMonsterSkill> GroundWave = GetWorld()->SpawnActor<AMonsterSkill>();
	GroundWave->ChangeAnimation("BossGroundWave");
	GroundWave->ChangeNextAnimation("BossGroundWaveLoop"); // ���� �ִϸ��̼��� ������ �ٲ� �ִϸ��̼�
	GroundWave->SetAutoRelease(false);
	GroundWave->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT) - 1);
	GroundWave->SetParentRoom(ParentRoom); // �ȼ��浹 �˻�
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
	bIsResting = true; // ���ϰ� ���� �� ��Ÿ�� ����

	ChangeNextState(EMonsterState::ATTACK_RECOVERY2);
}

void AFalseKnight::SetAttackRecovery2(float _DeltaTime)
{
	ActiveGravity();

	ResetRendererOffest();

	ChangeNextState(EMonsterState::IDLE);
}

