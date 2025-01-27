#include "PreCompile.h"
#include "HuskHornhead.h"

AHuskHornhead::AHuskHornhead()
{
}

AHuskHornhead::~AHuskHornhead()
{
}

void AHuskHornhead::BeginPlay()
{
	AMonster::BeginPlay();
}

void AHuskHornhead::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AHuskHornhead::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 0.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 15;
	Data.Hp = 5;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = Data.Velocity * 1.5f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackCooldown = 5.0f;
}

void AHuskHornhead::SetOffset()
{
	FVector ImageSize = { 2500.0f , 4000.0f };
	FVector SpriteSize = { 128.0f, 200.0f };

	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	//GravityPointOffset.Y = (ImageSize.Y - SpriteSize.Y) / 2.0f; // (이미지 크기 - 1프레임 크기) / 2.0f
	//WallPointOffest = { -((ImageSize.X / 2.0f) - SpriteSize.X * 1.5f), (ImageSize.Y / 2.0f) - SpriteSize.Y * 2.4f }; // 이미지마다 다 값이 다른듯

	CollisionScale = { 100, 200 };
	DetectRange = { 700, 50 };

	DeathSpriteOffset = -70.0f;
	DeathAirDistance = 300.0f;
	//DeathRotation = 1.0f;
}

void AHuskHornhead::CreateAnimation()
{
	SetName("HuskHornhead");
	std::string HuskHornhead = "HuskHornhead.png";
	float IdleTime = 0.2f;
	float TurnTime = 0.3f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float AttackTime = 0.08f;
	float RecoveryTime = 0.1f;
	float DeathAirTime = 0.5f;
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", HuskHornhead, 0, 5, IdleTime);
	BodyRenderer->CreateAnimation("Walk", HuskHornhead, 6, 12, RunnigTime);
	BodyRenderer->CreateAnimation("Turn", HuskHornhead, 13, 14, TurnTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", HuskHornhead, 15, 19, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", HuskHornhead, 20, 23, AttackTime, false);
	BodyRenderer->CreateAnimation("AttackRecovery", HuskHornhead, 24, 24, RecoveryTime, false);
	BodyRenderer->CreateAnimation("DeathAir", HuskHornhead, 25, 25, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", HuskHornhead, 26, 33, DeathTime, false);


	AttackDuration = AttackTime * 6.0f;
	BodyRenderer->ChangeAnimation("Idle");
}

void AHuskHornhead::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D(CollisionScale);
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

