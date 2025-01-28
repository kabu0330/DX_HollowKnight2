#include "PreCompile.h"
#include "AspidHunter.h"

AAspidHunter::AAspidHunter()
{
}

AAspidHunter::~AAspidHunter()
{
}

void AAspidHunter::BeginPlay()
{
	AMonster::BeginPlay();
}

void AAspidHunter::Tick(float _DeltaTime)
{
	AMonster::Tick(_DeltaTime);
}

void AAspidHunter::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 1.0f;
	Data.DashSpeed = Data.Velocity * 1.0f;
	Data.MaxHp = 15;
	Data.Hp = 5;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 300.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = 0.0f;
	InitJumpForce = 400.0f;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true
	bCanFly = true; // 날아다니는 몬스터라면 true

	MoveDuration = 2.0f;
	MoveCooldown = 5.0f;

	AttackFrame = 0.15f;// 매번 설정
	AttackDuration = AttackFrame * 12.0f;
	AttackCooldown = 5.0f;

	AttackRecoveryFrame = 0.1f;

	bIsDeathDestroy = true; // 죽으면 시체 없이 소멸할건지
}

void AAspidHunter::SetOffset()
{
	FVector ImageSize = { 713.0f , 410.0f };
	FVector SpriteSize = { 80.0f, 80.0f };

	DetectRange = { 1000, 1000 };
	CollisionScale = SpriteSize;

	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };

	DeathSpriteOffset = -30.0f;
	DeathAirDistance = 300.0f;

	WalkRendererOffset = { 0.0f, 0.0f };
	AttackRendererOffset = { 0.0f, 0.0f };
	//DeathRotation = 1.0f;
}

void AAspidHunter::CreateAnimation()
{
	SetName("AspidHunter");
	std::string MonsterStr = "AspidHunter.png";
	float IdleTime = 0.2f;
	float RunnigTime = 0.1f;
	float AttackAnticipateTime = 0.1f;
	float DeathAirTime = 0.1f;
	float DeathTime = 0.08f;
	BodyRenderer->CreateAnimation("Idle", MonsterStr, 0, 5, IdleTime);
	BodyRenderer->CreateAnimation("Walk", MonsterStr, 0, 5, RunnigTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", MonsterStr, 7, 10, AttackAnticipateTime);
	BodyRenderer->CreateAnimation("Attack", MonsterStr, 11, 15, AttackFrame);
	BodyRenderer->CreateAnimation("AttackRecovery", MonsterStr, 16, 18, AttackRecoveryFrame, false);
	BodyRenderer->CreateAnimation("DeathAir", MonsterStr, 19, 21, DeathAirTime);
	BodyRenderer->CreateAnimation("DeathLand", MonsterStr, 22, 24, DeathTime, false);


	BodyRenderer->ChangeAnimation("Idle");
}

void AAspidHunter::SetAttack(float _DeltaTime)
{
}
