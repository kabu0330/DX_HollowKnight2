#include "PreCompile.h"
#include "FalseKnight.h"
#include "MonsterEffect.h"
#include <EngineBase/EngineRandom.h>
#include "MonsterProjectile.h"
#include "FalseKnightHead.h"
#include "ContentsRenderer.h"
#include "PlayGameMode.h"
#include "Particle.h"

AFalseKnight::AFalseKnight()
{
}

AFalseKnight::~AFalseKnight()
{
}

void AFalseKnight::BeginPlay()
{
	AMonster::BeginPlay();
	CreateHeadRenderer();
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
	Data.Velocity = 400.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 1000;
	Data.Hp = 1000;
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
	FVector SpriteSize = { 290.0f, 280.0f };

	bIsFlip = true; // ������ �¿����

	BodyCollisionOffset = { 60.0f, -10.0f }; // �ø��� ��ġ�� ���� �ȼ��浹 ��ġ�� �ٲ��.
	RendererOffset = { 0.0f, 40.0f };

	CollisionScale = SpriteSize;
	DetectRange = { 4000, 1000 };

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
	float StunTime = 0.15f;
	BodyRenderer->SetName("FalseKnight");
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 3, IdleTime);
	BodyRenderer->CreateAnimation("JumpAnticipate", MonsterStr, 13, 15, RunnigTime, false);
	BodyRenderer->CreateAnimation("Jump", MonsterStr, 16, 19, JumpTime, false);
	BodyRenderer->CreateAnimation("Jumping", MonsterStr, 19, 19, JumpTime, false);
	BodyRenderer->CreateAnimation("Land", MonsterStr, 20, 22, JumpTime, false);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 23, 28, AttackAnticipateTime, false);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 29, 31, AttackFrame, false);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 32, 34, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("AttackRecovery2", MonsterStr, 35, 36, AttackRecoveryFrame, false);

	BodyRenderer->CreateAnimation("BerserkAttackAnticipate", MonsterStr, 23, 28, AttackAnticipateTime, false);
	BodyRenderer->CreateAnimation("BerserkAttack", MonsterStr, 29, 31, DeathAirTime, false);
	BodyRenderer->CreateAnimation("BerserkAttack2", MonsterStr, 29, 31, DeathAirTime, false);
	BodyRenderer->CreateAnimation("BerserkAttackRecovery", MonsterStr, 32, 34, DeathAirTime, false);
	BodyRenderer->CreateAnimation("BerserkAttackRecovery2", MonsterStr, 35, 36, AttackRecoveryFrame, false);

	BodyRenderer->CreateAnimation("JumpAttackAnticipate", MonsterStr, 37, 42, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("JumpAttack", MonsterStr, 43, 43, AttackAnticipateTime, false);
	BodyRenderer->CreateAnimation("JumpAttackRecovery", MonsterStr, 44, 45, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("JumpAttackLand", MonsterStr, 46, 47, AttackRecoveryFrame, false);

	BodyRenderer->CreateAnimation("Stun", MonsterStr, 48, 61, DeathAirTime, false);
	BodyRenderer->CreateAnimation("StunOpen", MonsterStr, 62, 62, AttackAnticipateTime, false); // 62
	BodyRenderer->CreateAnimation("StunHit", MonsterStr, 62, 62, DeathTime, false);
	BodyRenderer->CreateAnimation("StunRecovery", MonsterStr, {61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48}, DeathAirTime, false);

	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 72, 79, AttackAnticipateTime, false);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 80, 80, DeathTime, false); //75

	BodyRenderer->ChangeAnimation("Idle");
}

void AFalseKnight::CreateHeadRenderer()
{
	Head = GetWorld()->SpawnActor<AFalseKnightHead>().get();

	Head->ChangeAnimation("Stun");
	Head->ToggleFlip();
	Head->SetParent(this);

	FVector CollisionScale = FVector(120, 120);
	Head->SetCollisionScale(CollisionScale);
	Head->SetActive(false);
}

void AFalseKnight::DamageLogic(int _KnightAtt)
{
	if (0 >= PhaseHp)
	{
		bIsStun = true;
	}
	PhaseHp -= _KnightAtt;
}

void AFalseKnight::ChangeStunAnimation()
{
	if (true == bIsStun)
	{
		FSM.ChangeState(EMonsterState::STUN);
		return;
	}
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

	// ����ȭ ����
	if (false == bCanBerserkAttack)
	{
		BerserkAttackElapsed += _DeltaTime;
		float Cooldown = 20.0f;
		if (BerserkAttackElapsed >= Cooldown)
		{
			BerserkAttackElapsed = 0.0f;
			bCanBerserkAttack = true;
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
	ResetRendererOffset();

	ChangeStunAnimation();


	ActiveGravity();

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

	// ����
	// ���� 1. ���鰭Ÿ
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanAttack) // �÷��̾� ����
	{
		FSM.ChangeState(EMonsterState::ATTACK_ANTICIPATE);
	}
	// ���� 2. ����, 3. ���� ����
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanJump) 
	{
		UEngineRandom Random;
		float Result = Random.Randomfloat(-300.0f, 400.0f);
		Stat.SetVelocity(Result);
		FSM.ChangeState(EMonsterState::JUMP_ANTICIPATE);
	}
	// ���� 4. ����ȭ ����
	if (true == IsPlayerNearby() && false == Stat.IsAttacking() && true == bCanBerserkAttack && true == bIsBerserkMode)
	{
		FSM.ChangeState(EMonsterState::BERSERK_ATTACK_ANTICIPATE);
	}
}

void AFalseKnight::SetJumpAnticipate(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	Stat.SetAttacking(true);
	bCanRotation = false;
	bCanJump = false;

	UEngineRandom Random;
	int Result = Random.RandomInt(0, 1);
	if (0 == Result)
	{
		ChangeNextState(EMonsterState::JUMP);
	}
	else
	{
		ChangeNextState(EMonsterState::JUMP_ATTACK_ANTICIPATE);
	}
}

void AFalseKnight::SetJump(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

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

	ChangeStunAnimation();

	JumpForce = 0.0f;
	bIsResting = true; // ���ϰ� ���� �� ��Ÿ�� ����

	ChangeNextState(EMonsterState::IDLE);
}

void AFalseKnight::SetJumpAttackAnticipate(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	Move(_DeltaTime);
	FVector Offset = { 50.0f, 60.0f }; 
	SetRendererOffset(Offset);

	JumpActionInitElapsed += _DeltaTime;
	float JumpActionTime = 1.0f;
	if (JumpActionInitElapsed <= JumpActionTime)
	{
		Jump(_DeltaTime);
	}
	else if (JumpActionInitElapsed >= JumpActionTime && true == bIsOnGround)
	{
		JumpActionInitElapsed = 0.0f;
		FSM.ChangeState(EMonsterState::JUMP_ATTACK);
	}
}

void AFalseKnight::SetJumpAttack(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	FVector Offset = { -150.0f, 90.0f }; 
	SetRendererOffset(Offset);

	CreateJumpAttackLogicAndEffect();

	ChangeNextState(EMonsterState::JUMP_ATTACK_RECOVERY);
}

void AFalseKnight::CreateJumpAttackLogicAndEffect()
{
	// �ø��� ����
	std::shared_ptr<AMonsterSkill> Skill = GetWorld()->SpawnActor<AMonsterSkill>();

	//Skill->SetCollisionTime(AttackDuration);
	Skill->SetCollisionTime(0.1f);
	Skill->SetParentMonster(this); // �¿� ���� ������ ����

	FVector CollisionScale = FVector(200, 200);
	Skill->SetCollisionScale(CollisionScale);

	// ����Ʈ ����
	AMonsterEffect* Effect = GetWorld()->SpawnActor<AMonsterEffect>().get();
	FVector Pos = GetActorLocation();
	Effect->ChangeAnimation("GroundImapctEffect", { Pos.X, Pos.Y });
	Effect->SetScale(1.5f);

	FVector Offset = FVector{ -330.0f, -100.0f };
	FVector EffectOffset = FVector(0.0f, 20.0f);
	if (true == bIsLeft)
	{
		Skill->SetLocation(this, { Offset.X, Offset.Y });
		Effect->SetLocation(this, { Offset.X, Offset.Y + EffectOffset.Y });
	}
	else
	{
		Skill->SetLocation(this, { Offset.X, -Offset.Y });
		Effect->SetLocation(this, { Offset.X, -(Offset.Y + EffectOffset.Y) });
	}
}

void AFalseKnight::SetJumpAttackRecovery(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	FVector Offset = { 0.0f, 50.0f }; 
	SetRendererOffset(Offset);

	ChangeNextState(EMonsterState::JUMP_ATTACK_LAND);
}

void AFalseKnight::SetJumpAttackLand(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	FVector Offset = { 20.0f, 0.0f }; 
	SetRendererOffset(Offset);

	bIsResting = true; // ���ϰ� ���� �� ��Ÿ�� ����
	ChangeNextState(EMonsterState::IDLE);
}

void AFalseKnight::SetAttackAnticipate(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	// ������ ��ġ ����
	FVector Offset = { 40.0f, -20.0f };
	SetRendererOffset(Offset);
	
	Stat.SetAttacking(true);
	bCanAttack = false;
	bCanRotation = false;
	ChangeNextState(EMonsterState::ATTACK);
}

void AFalseKnight::SetAttack(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	// ������ ��ġ ����
	if (false == bIsOffsetAttack1Frame)
	{
		FVector Offset = { -70.0f, 80.0f };
		SetRendererOffset(Offset);
		bIsOffsetAttack1Frame = true;
	}
	if (false == bIsShowEffect) // �� ���� ���� ���� ȣ��
	{
		TimeEventer->AddEndEvent(0.15f, [this]()
			{
				FVector Offset = { -160.0f, 20.0f };
				SetRendererOffset(Offset);
				CreateAttackLogicAndEffect();
				TimeEventer->AddEndEvent(0.3f, std::bind(&AFalseKnight::CreateGroundImpack, this));
				CreateDropObject();
				CreateDropObject();
				CreateDropObject();
			});
	}
	bIsShowEffect = true;

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			FVector Offset = { -160.0f, -45.0f };
			SetRendererOffset(Offset);
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
	Skill->SetParentMonster(this); // �¿� ���� ������ ����

	FVector CollisionScale = FVector(150, 150);
	Skill->SetCollisionScale(CollisionScale);

	// ����Ʈ ����
	AMonsterEffect* Effect = GetWorld()->SpawnActor<AMonsterEffect>().get();
	FVector Pos = GetActorLocation();
	Effect->ChangeAnimation("GroundImapctEffect", { Pos.X, Pos.Y });
	Effect->SetScale(1.5f);
	
	FVector Offset = FVector{ -330.0f, -100.0f };
	FVector EffectOffset = FVector(0.0f, 20.0f);
	if (true == bIsLeft)
	{
		Skill->SetLocation(this, { Offset.X, Offset.Y });
		Effect->SetLocation(this, { Offset.X, Offset.Y + EffectOffset.Y});
	}
	else
	{
		Skill->SetLocation(this, { Offset.X, -Offset.Y });
		Effect->SetLocation(this, { Offset.X, -(Offset.Y + EffectOffset.Y) });
	}	
}

void AFalseKnight::CreateGroundImpack()
{
	// �׶��� ���̺�(�̵��ϴ� �����) ����
	std::shared_ptr<AMonsterSkill> GroundWave = GetWorld()->SpawnActor<AMonsterSkill>();
	GroundWave->ChangeAnimation("BossGroundWave");
	GroundWave->ChangeNextAnimation("BossGroundWaveLoop"); // ���� �ִϸ��̼��� ������ �ٲ� �ִϸ��̼�
	GroundWave->SetParentMonster(this);
	GroundWave->SetAutoRelease(false);
	GroundWave->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT) - 1);
	GroundWave->SetParentRoom(ParentRoom); // �ȼ��浹 �˻�
	GroundWave->SetCollisionTime(AttackDuration * 5.0f);
	GroundWave->SetCollisionScale({ 180.0f, 50.0f });
	GroundWave->SetScale(1.3f);

	FVector GroundWaveOffset = FVector(-330.0f, -70.0f);
	FVector Speed = FVector(800.0f, 0.0f);
	if (true == bIsLeft)
	{
		GroundWave->AddLocation(this, -Speed, GroundWaveOffset);
	}
	else
	{
		GroundWave->AddLocation(this, Speed, { GroundWaveOffset.X, -GroundWaveOffset.Y });
	}
}

void AFalseKnight::CreateDropObject()
{
	if (false == bIsBerserkMode)
	{
		return;
	}

	AMonsterProjectile* DropObject = GetWorld()->SpawnActor<AMonsterProjectile>().get();
	DropObject->ChangeAnimation("BossDropObject");
	DropObject->SetParentMonster(this);
	DropObject->SetAutoRelease(false);
	DropObject->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_FRONT) - DropObjectCount);
	DropObject->SetParentRoom(ParentRoom); // �ȼ��浹 �˻�
	DropObject->SetCollisionTime(AttackDuration * 5.0f);
	DropObject->SetCollisionScale({ 157.0f * 0.6f, 143.0f * 0.6f });
	DropObject->SetScale(0.7f);

	FVector ScreenSize = UEngineCore::GetScreenScale();
	FVector ScreenHalfSize = ScreenSize.Half();

	std::random_device rd;
	std::mt19937_64 RandomGen(rd() + static_cast<__int64>(DropObjectCount));
	std::uniform_real_distribution<float> Dist(-ScreenSize.X * 1.5f, ScreenSize.X * 1.5f);
	std::uniform_real_distribution<float> Dist2(-550.0f, -350.0f);

	FVector Offset = FVector{ Dist(RandomGen), ScreenHalfSize.Y + 300.0f };
	FVector Speed = FVector(0.0f, Dist2(RandomGen));
	if (true == bIsLeft)
	{
		DropObject->AddLocation(this, -Speed, Offset);
	}
	else
	{
		DropObject->AddLocation(this, Speed, { Offset.X, -Offset.Y });
	}


	++DropObjectCount;
	if (50 >= DropObjectCount)
	{
		DropObjectCount = 1;
	}
}

void AFalseKnight::SetAttackRecovery(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	ResetRendererOffset();
	FVector Offset = { 0.0f, 60.0f };
	SetRendererOffset(Offset);

	bIsShowEffect = false;
	bIsOffsetAttack1Frame = false;
	bIsResting = true; // ���ϰ� ���� �� ��Ÿ�� ����

	ChangeNextState(EMonsterState::ATTACK_RECOVERY2);
}

void AFalseKnight::SetAttackRecovery2(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	ResetRendererOffset();

	ChangeNextState(EMonsterState::IDLE);
}

void AFalseKnight::SetBerserkAttackAnticipate(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	bCanBerserkAttack = false;

	// ������ ��ġ ����
	FVector Offset = { 40.0f, -20.0f };
	SetRendererOffset(Offset);

	Stat.SetAttacking(true);
	bCanAttack = false;
	bCanRotation = false;
	ChangeNextState(EMonsterState::BERSERK_ATTACK);
}

void AFalseKnight::SetBerserkAttack(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();
	// ������ ��ġ ����
	if (false == bIsOffsetAttack1Frame)
	{
		if (1 >= BerserkAttackCount)
		{
			bIsLeft = !bIsLeft;
			bCanRotation = true; // ������ȯ ���
			CheckDirection(); // �¿� ���� ����
		}
		++BerserkAttackCount;

		FVector Offset = { -70.0f, 83.0f };
		SetRendererOffset(Offset);
		bIsOffsetAttack1Frame = true;
	}
	if (false == bIsShowEffect) // �� ���� ���� ���� ȣ��
	{
		TimeEventer->AddEndEvent(0.1f, [this]()
			{
				FVector Offset = { -160.0f, 20.0f };
				SetRendererOffset(Offset);
				CreateAttackLogicAndEffect();
				CreateDropObject();
				CreateDropObject();
				CreateDropObject();
				CreateDropObject();
			});
	}
	bIsShowEffect = true;

	TimeEventer->AddEndEvent(0.2f, [this]()
		{
			FVector Offset = { -160.0f, -45.0f };
			SetRendererOffset(Offset);
		});

	ChangeNextState(EMonsterState::BERSERK_ATTACK_RECOVERY);
}

void AFalseKnight::SetBerserkAttack2(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	// ������ ��ġ ����
	if (false == bIsOffsetAttack1Frame)
	{
		++BerserkAttackCount;

		bIsLeft = !bIsLeft;
		bCanRotation = true; // ������ȯ ���
		CheckDirection(); // �¿� ���� ����

		FVector Offset = { -70.0f, 83.0f };
		SetRendererOffset(Offset);
		bIsOffsetAttack1Frame = true;
	}
	if (false == bIsShowEffect) // �� ���� ���� ���� ȣ��
	{
		TimeEventer->AddEndEvent(0.1f, [this]()
			{
				FVector Offset = { -160.0f, 20.0f };
				SetRendererOffset(Offset);
				CreateAttackLogicAndEffect();
				CreateDropObject();
				CreateDropObject();
				CreateDropObject();
				CreateDropObject();
			});
	}
	bIsShowEffect = true;

	TimeEventer->AddEndEvent(0.2f, [this]()
		{
			FVector Offset = { -160.0f, -45.0f };
			SetRendererOffset(Offset);
		});

	ChangeNextState(EMonsterState::BERSERK_ATTACK_RECOVERY);
}

void AFalseKnight::SetBerserkAttackRecovery(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	ResetRendererOffset();
	FVector Offset = { 0.0f, 60.0f };
	SetRendererOffset(Offset);

	bIsShowEffect = false;
	bIsOffsetAttack1Frame = false;
	
	if (BerserkAttackCount > BerserkAttackCountMax)
	{
		ChangeNextState(EMonsterState::BERSERK_ATTACK_RECOVERY_2);
	}
	else
	{
		if (1 == BerserkAttackCount % 2)
		{
			ChangeNextState(EMonsterState::BERSERK_ATTACK);
			return;
		}
		else
		{
			ChangeNextState(EMonsterState::BERSERK_ATTACK_2);
			return;
		}
	}
}

void AFalseKnight::SetBerserkAttackRecovery2(float _DeltaTime)
{
	ActiveGravity();

	ChangeStunAnimation();

	bIsResting = true; // ���ϰ� ���� �� ��Ÿ�� ����
	BerserkAttackCount = 0;
	ResetRendererOffset();

	ChangeNextState(EMonsterState::IDLE);
}

void AFalseKnight::SetStun(float _DeltaTime)
{
	ActiveGravity();

	ResetRendererOffset();
	BodyCollision->SetActive(false); // ��ü�� ����
	bIsInit = true;

	FVector Offset = { 0.0f, 30.0f };
	SetRendererOffset(Offset);

	CreateStunEffect();

	ChangeNextState(EMonsterState::STUN_OPEN);
}

void AFalseKnight::CreateStunEffect()
{
	if (true == bIsStunEffect)
	{
		return;
	}
	bIsStunEffect = true;

	AEffect* WhiteEffect = GetWorld()->SpawnActor<AEffect>().get();
	WhiteEffect->SetZSort(static_cast<int>(EZOrder::MONSTER_SKILL_FRONT) - 6);
	WhiteEffect->SetName("Stun White Effect");
	WhiteEffect->ChangeAnimation("StunEffect03", GetActorLocation());
	WhiteEffect->SetScale(2.5f);
	WhiteEffect->SetLocation(this);
}

void AFalseKnight::SetStunOpen(float _DeltaTime)
{
	ActiveGravity();
	bIsStunEffect = false;

	if (true == bIsInit) // ���� ���� ���� 1ȸ
	{
		bIsInit = false;
		bIsBerserkMode = true; // ���� ���Ϻ��� ����ȭ ��带 �ߵ�

		Head->SetActive(true);
		Head->SetDamage(false);

		// ��� ����
		FVector Offset = { 135.0f, -100.0f };
		if (true == bIsLeft)
		{
			Head->SetLocation(this, { -Offset.X, Offset.Y });
		}
		else
		{
			Head->SetLocation(this, { -Offset.X, -Offset.Y });
		}
	}
}

void AFalseKnight::SetStunHit(float _DeltaTime)
{
	ActiveGravity();
	UEngineDebug::OutPutString("���� ��Ʈ" + std::to_string(HeadHp));

	Head->GetRenderer()->ChangeAnimation("StunHit");
	if (0 >= HeadHp)
	{
		bCanStunRecovery = false;

		// ���� �ʱ�ȭ
		bIsStun = false; // ���� ����

		--DeathCountValue;
		if (0 >= DeathCountValue)
		{
			bIsDeathAir = true;
			FSM.ChangeState(EMonsterState::DEATH_AIR);
			return;
		}
		else
		{
			bIsDeathAir = false;
			FSM.ChangeState(EMonsterState::STUN_RECOVERY);
			return;
		}
	}
	else
	{
		TimeEventer->AddEndEvent(0.3f, [this]()
			{
				bIsInit = true;
				Head->GetRenderer()->ChangeAnimation("Stun");
				FSM.ChangeState(EMonsterState::STUN_OPEN);
			});
	}
}

void AFalseKnight::SetStunRecovery(float _DeltaTime)
{
	ActiveGravity();

	if (false == bIsDeathAir)
	{
		HeadHp = 40;
		PhaseHp = 75;

		Head->SetActive(false);

		TimeEventer->AddEndEvent(1.5f, [this]()
			{
				BodyCollision->SetActive(true);
			});
	}

	ChangeNextState(EMonsterState::IDLE);	
}

void AFalseKnight::SetDeathAir(float _DeltaTime)
{
	ActiveGravity();
	BodyCollision->SetActive(false);
	Head->SetActive(false);
	bIsDeath = true;

	JumpActionInitElapsed += _DeltaTime;
	float JumpActionTime = 1.0f;
	if (JumpActionInitElapsed <= JumpActionTime)
	{
		Jump(_DeltaTime);
	}
	if (true == bIsOnGround && true == BodyRenderer->IsCurAnimationEnd())
	{
		TimeEventer->AddEvent(3.0f, std::bind(&AFalseKnight::CreateDeathEffect, this, std::placeholders::_1), [this]()
			{
				FSM.ChangeState(EMonsterState::DEATH_LAND);
			});
	}
}

void AFalseKnight::CreateDeathEffect(float _DeltaTime)
{
	DeathEffectTimeElapesd += _DeltaTime;
	float Cooldown = 0.3f;
	if (DeathEffectTimeElapesd >= Cooldown)
	{
		CreateDeathOrangeParticleEffect();
		CreateDeathOrangeParticleEffect();
		DeathEffectTimeElapesd = 0.0f;
	}	
}

void AFalseKnight::CreateDeathOrangeParticleEffect()
{
	AParticle* PuffParticle = GetWorld()->SpawnActor<AParticle>().get();
	FVector ActorPos = GetActorLocation();
	PuffParticle->CreateParticle("Puff", 10, 0.01f, ActorPos);
	PuffParticle->SetParticleOption(EParticleType::RANDOM, -1200.0f, 1200.0f);

	AParticle* OrangeParticle = GetWorld()->SpawnActor<AParticle>().get();
	OrangeParticle->CreateParticle("OrangeParticle", 20, 0.01f, ActorPos);
	OrangeParticle->SetRandomScale(0.5f, 1.0f);
	OrangeParticle->SetDecayScale(true, 0.6f);
	OrangeParticle->SetParticleOption(EParticleType::RANDOM, -1200.0f, 1200.0f);

	AParticle* Particle = GetWorld()->SpawnActor<AParticle>().get();
	Particle->CreateParticle("DefaultHitParticle", 20, 0.01f, ActorPos);
	Particle->SetParticleOption(EParticleType::RANDOM, -1200.0f, 1200.0f);
}

void AFalseKnight::SetDeathLand(float _DeltaTime)
{
	ActiveGravity();

	if (true == bIsDeath) // �� �ѹ��� ȣ��
	{
		ResetRendererOffset();
		FVector Offset = { 0.0f, -75.0f };
		SetRendererOffset(Offset);

		Head->SetActive(true);
		Head->GetCollision()->SetActive(false);
		ResetRendererOffset(Head->GetRenderer());
		Head->ChangeAnimation("Death");
		Head->SetScale(0.8f);

		FVector HeadOffset = { 260.0f, -145.0f };
		if (true == bIsLeft)
		{
			Head->SetLocation(this, { -HeadOffset.X, HeadOffset.Y });
		}
		else
		{
			Head->SetLocation(this, { -HeadOffset.X, -HeadOffset.Y });
		}

		bIsDeath = false;
	}
}

