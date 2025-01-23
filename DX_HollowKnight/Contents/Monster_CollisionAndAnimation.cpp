#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"

void AMonster::CreateAnimation()
{
	//SetName("Monster");
	//std::string AWanderingHusk = "WanderingHusk.png";
	//float IdleTime = 0.2f;
	//float TurnTime = 0.3f;
	//float RunnigTime = 0.1f;
	//float AttackAnticipateTime = 0.15f;
	//float AttackTime = 0.12f;
	//float RecoveryTime = 0.1f;
	//float DeathAirTime = 0.2f;
	//float DeathTime = 0.15f;
	//BodyRenderer->CreateAnimation("Idle", AWanderingHusk, 0, 5, IdleTime);
	//BodyRenderer->CreateAnimation("Walk", AWanderingHusk, 6, 12, RunnigTime);
	//BodyRenderer->CreateAnimation("Turn", AWanderingHusk, 13, 14, TurnTime);
	//BodyRenderer->CreateAnimation("AttackAnticipate", AWanderingHusk, 15, 19, AttackAnticipateTime);
	//BodyRenderer->CreateAnimation("Attack", AWanderingHusk, 20, 23, AttackTime);
	//BodyRenderer->CreateAnimation("AttackRecovery", AWanderingHusk, 24, 24, RecoveryTime, false);
	//BodyRenderer->CreateAnimation("DeathAir", AWanderingHusk, 25, 25, DeathAirTime);
	//BodyRenderer->CreateAnimation("DeathLand", AWanderingHusk, 26, 33, DeathTime, false);

	//BodyRenderer->ChangeAnimation("Idle");
}

void AMonster::SetCollisionEvent()
{
	BodyCollision->SetCollisionEnter(std::bind(&AMonster::OnBodyCollision, this, std::placeholders::_1, std::placeholders::_2));
	DetectCollision->SetCollisionStay([this](UCollision* _This, UCollision* _Other) {
		bIsChasing = true;
		});
	DetectCollision->SetCollisionEnd([this](UCollision* _This, UCollision* _Other) {
		bIsChasing = false;
		});
}

void AMonster::OnBodyCollision(UCollision* _This, UCollision* _Other)
{
	// Debug
	return;

	AActor* Other = _Other->GetActor();
	AActor* Actor = dynamic_cast<AActor*>(Other);
	if (nullptr == Actor)
	{
		return;
	}
	AKnight* OtherKnight = dynamic_cast<AKnight*>(Actor);
	if (nullptr == OtherKnight)
	{
		return;
	}
	int Att = Stat.GetAtt();

	UFightUnit::OnHit(OtherKnight, Att);
	CancleCurAction();

	int KnightCurHp = OtherKnight->GetStatRef().GetHp();
	UEngineDebug::OutPutString(GetName() + "의 공격으로 나이트가 " + std::to_string(Att) + "의 피해를 입었습니다. 남은 체력 : " + std::to_string(KnightCurHp));
}

void AMonster::CancleCurAction()
{
	Stat.SetBeingHit(true);
}

void AMonster::CreateCollision()
{
	// Collision
	//BodyCollision = CreateDefaultSubObject<UCollision>();
	//BodyCollision->SetupAttachment(RootComponent);
	//BodyCollision->SetScale3D({ 106, 127 });
	//BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	//BodyCollision->SetCollisionProfileName("Monster");
}

void AMonster::CreateDetectCollision()
{
	DetectCollision = CreateDefaultSubObject<UCollision>();
	DetectCollision->SetupAttachment(RootComponent);
	DetectCollision->SetScale3D(DetectRange);
	DetectCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	DetectCollision->SetCollisionProfileName("MonsterDetect");
	DetectCollision->SetDebugColor({ 1.0f, 1.0f, 0.0f });
}

void AMonster::CreateCenterPoint()
{
	CenterPoint = CreateDefaultSubObject<UCollision>();
	CenterPoint->SetupAttachment(RootComponent);
	CenterPoint->SetScale3D({ 10, 10 });
	CenterPoint->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	CenterPoint->SetCollisionProfileName("MonsterPoint");
	CenterPoint->SetDebugColor({ 1.0f, 0.0f, 0.0f });
}

void AMonster::CreatePixelCollision()
{
	PixelBot = CreateDefaultSubObject<UCollision>();
	PixelBot->SetupAttachment(RootComponent);
	PixelBot->SetScale3D({ 100, 100 });
	PixelBot->SetCollisionProfileName("MonsterPoint");

	FVector Pos = ParentRoom->GetPixelCollisionPoint(this, BodyRenderer.get(), GravityPointOffset);
	PixelBot->SetWorldLocation({Pos.X, Pos.Y, ZSort});
	PixelBot->SetDebugColor({ 1.0f, 1.0f, 1.0f });
}

void AMonster::ActivePixelCollision()
{
	if (true == IsCurRoom())
	{
		ParentRoom->CheckPixelCollisionWithWall(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft, WallPointOffest);
		ParentRoom->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft, CeilPointOffset);
	}
}

void AMonster::ActiveGravity()
{
	if (true == IsCurRoom())
	{
		ParentRoom->CheckPixelCollisionWithGravity(this, BodyRenderer.get(), GravityPointOffset);
	}
}

void AMonster::EnforceGravity(float _DeltaTime)
{
	if (true == IsCurRoom())
	{
		ParentRoom->EnforceGravity(this, _DeltaTime);
	}
}
