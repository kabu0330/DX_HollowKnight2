#include "PreCompile.h"
#include "LeapingHusk.h"

ALeapingHusk::ALeapingHusk()
{
}

ALeapingHusk::~ALeapingHusk()
{
}

void ALeapingHusk::BeginPlay()
{
	AMonster::BeginPlay();
}

void ALeapingHusk::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void ALeapingHusk::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 15;
	Data.Hp = 15;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = Data.Velocity * 2.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = InitJumpForce;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackDuration = 1.0f;
	AttackCooldown = 5.0f;
}

void ALeapingHusk::SetOffset()
{
	FVector ImageSize = { 2500.0f , 4000.0f};
	FVector SpriteSize = { 128.0f, 200.0f };
	
	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	GravityPointOffset.Y = (ImageSize.Y - SpriteSize.Y) / 2.0f; // (이미지 크기 - 1프레임 크기) / 2.0f
	WallPointOffest = { -((ImageSize.X / 2.0f) - SpriteSize.X * 1.5f), (ImageSize.Y  / 2.0f) - SpriteSize.Y * 2.4f};

	DetectRange = { 700, 50 };
}

void ALeapingHusk::CreateAnimation()
{
	SetName("Monster");
	std::string ALeapingHusk = "LeapingHusk.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.15f;
	float AttackTime = 0.12f;
	float RecoveryTime = 0.2f;
	float DeathAirTime = 0.2f;
	float DeathTime = 0.15f;
	BodyRenderer->CreateAnimation("Idle", ALeapingHusk, 0, 5, IdleTime);
	BodyRenderer->CreateAnimation("Walk", ALeapingHusk, 6, 12, RunnigTime);
	BodyRenderer->CreateAnimation("Turn", ALeapingHusk, 13, 14, TurnTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", ALeapingHusk, 15, 17, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", ALeapingHusk, 20, 25, AttackTime);
	BodyRenderer->CreateAnimation("AttackRecovery", ALeapingHusk, 26, 27, RecoveryTime, false);
	BodyRenderer->CreateAnimation("DeathAir", ALeapingHusk, 28, 28, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", ALeapingHusk, 29, 36, DeathTime, false);

	BodyRenderer->ChangeAnimation("Idle");
}

void ALeapingHusk::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D({ 106, 127 });
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

