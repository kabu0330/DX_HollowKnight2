#include "PreCompile.h"
#include "AspidHunter.h"
#include "MonsterProjectile.h"

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
	Data.Velocity = 0.0f;
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
	float DeathAirTime = 0.12f;
	float DeathTime = 0.12f;
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
	SetAttackRendererOffset();
	CheckDeath();
	ActiveGravity();

	//Dash();

	// 가상함수
	CreateAttackLogicAndEffect();

	AttackFrameElapsed += _DeltaTime;
	//   피격시                        넉백이 적용되는 친구들은 모두 스킬 캔슬
	if (true == Stat.IsBeingHit() && true == Stat.IsKnockbackable())
	{
		bIsFire = false;
		bIsFirstIdle = true; // Idle로 돌아갈때 반드시 넣어주기
		ResetAttackCooldown();
		FSM.ChangeState(EMonsterState::IDLE);
	}
	else if (AttackFrameElapsed >= AttackDuration)
	{
		AttackFrameElapsed = 0.0f;
		bIsFire = false;
		FSM.ChangeState(EMonsterState::ATTACK_RECOVERY);
	}
}

void AAspidHunter::CreateAttackLogicAndEffect()
{
	if (true == bIsFire)
	{
		return;
	}
	bIsFire = true;

	std::shared_ptr<AMonsterProjectile> Projectile = GetWorld()->SpawnActor<AMonsterProjectile>();
	Projectile->SetZSort(EZOrder::MONSTER_SKILL_FRONT);
	Projectile->ChangeAnimation(this, "BulletFire");
	Projectile->SetParentRoom(ParentRoom);
	Projectile->GetRenderer()->SetAutoScale(false);
	FVector BulletSize = { 40.0f, 30.0f };
	Projectile->GetRenderer()->SetRelativeScale3D(BulletSize);
	Projectile->GetRenderer()->SetActive(true);
	Projectile->SetCollisionScale(BulletSize);
	Projectile->SetActorLocation(GetActorLocation()); 

	FVector ThisPos = GetActorLocation();
	FVector KnightPos = AKnight::GetPawn()->GetActorLocation();
	FVector Dir = KnightPos - ThisPos;
	Dir.Normalize();
	float Speed = 100.0f;
	FVector Offset = Dir * Speed;

	Projectile->AddLocation(this, Offset);

	//AKnightFireballEffect* Effect = GetWorld()->SpawnActor<AKnightFireballEffect>().get();
	//Effect->SetName("FireballWallImpact");
	//Effect->SetZSort(EZOrder::KNIGHT_SKILL_FRONT);
	//AKnight* Knight = AKnight::GetPawn();
	//Effect->ChangeAnimation("FireballWallImpact"); // RootComponent가 없다고 자꾸 터지는데 나이트 넣어주면 된다.
	//Effect->SetScale(1.5f);
	//Effect->ToggleFlip();
	//FVector Offset = { 50.0f, 0.0f };
	//if (nullptr != Collision && true == Collision->IsActive())
	//{
	//	if (true == bIsLeft)
	//	{
	//		Offset *= -1.0f;
	//	}
	//	PointPos = Collision->GetWorldLocation() + Offset;
	//}

	//Effect->SetLocation(PointPos);
	//Effect->GetRenderer()->SetMulColor({ 12.0f, 12.0f, 12.0f }, 0.1f);
}

