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
}

void AFalseKnight::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 0.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 15;
	Data.Hp = 10;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = false;
	Data.KnockbackDistance = 0.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // �⺻ ȸ�� ����
	bCanJump = false; // �����ϴ� ���͸� true
	bIsAggressive = false; // ȣ�����̸� true
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

	BodyCollisionOffset = { 60.0f, 0.0f }; // �ø��� ��ġ�� ���� �ȼ��浹 ��ġ�� �ٲ��.
	RendererOffset = { 0.0f, 40.0f };

	CollisionScale = SpriteSize;
	DetectRange = { 0, 0 };

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
	float JumpTime = 0.15f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.1f; // ���ư��� �ð� ������ ���� �ڿ��������� ǥ���ȴ�.
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 4, IdleTime);
	BodyRenderer->CreateAnimation("WalkAnticipate", MonsterStr, 7, 8, RunnigTime, false);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 9, 13, RunnigTime);
	BodyRenderer->CreateAnimation("JumpAnticipate", MonsterStr, 14, 16, RunnigTime);
	BodyRenderer->CreateAnimation("Jump", MonsterStr, 17, 20, JumpTime, false);
	BodyRenderer->CreateAnimation("Land", MonsterStr, 21, 23, JumpTime, false);
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

