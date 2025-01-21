#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"

void AMonster::CreateAnimation()
{
	// Renderer
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	BodyRenderer->SetWorldLocation({ RendererOffset.X, RendererOffset.Y, ZSort });

	std::string HuskBully = "HuskBully.png";
	float FrameTime = 0.2f;
	float RunnigTime = 0.1f;
	BodyRenderer->CreateAnimation("Idle", HuskBully, 0, 5, FrameTime);
	BodyRenderer->CreateAnimation("Walk", HuskBully, 6, 12, RunnigTime);
	BodyRenderer->CreateAnimation("Turn", HuskBully, 13, 14, FrameTime);
	BodyRenderer->CreateAnimation("AttackAnticipate", HuskBully, 15, 18, FrameTime);
	BodyRenderer->CreateAnimation("Attack", HuskBully, 19, 20, FrameTime);
	BodyRenderer->CreateAnimation("AttackRecovery", HuskBully, 21, 21, FrameTime);
	BodyRenderer->CreateAnimation("DeathAir", HuskBully, 22, 22, FrameTime);
	BodyRenderer->CreateAnimation("DeathLand", HuskBully, 23, 30, FrameTime);

	BodyRenderer->ChangeAnimation("Idle");
}

void AMonster::ChangeNextAnimation(EMonsterState _NextState)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(_NextState);
		return;
	}
}

void AMonster::ChangePrevAnimation()
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(NextState);
		return;
	}
}

void AMonster::SetCollisionEvent()
{
	BodyCollision->SetCollisionEnter(std::bind(&AMonster::OnBodyCollision, this, std::placeholders::_1, std::placeholders::_2));
	DetectCollision->SetCollisionStay([](UCollision* _This, UCollision* _Other) {
		//UEngineDebug::OutPutString("몬스터 탐색 범위 진입");
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
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D({ 106, 127 });
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

void AMonster::CreateCenterPoint()
{
	CenterPoint = CreateDefaultSubObject<UCollision>();
	CenterPoint->SetupAttachment(RootComponent);
	CenterPoint->SetScale3D({ 10, 10 });
	CenterPoint->SetCollisionProfileName("MonsterPoint");
	CenterPoint->SetDebugColor({ 1.0f, 0.0f, 0.0f });
}

void AMonster::CreateDetectCollision()
{
	DetectCollision = CreateDefaultSubObject<UCollision>();
	DetectCollision->SetupAttachment(RootComponent);
	DetectCollision->SetScale3D(DetectRange);
	DetectCollision->SetCollisionProfileName("MonsterDetect");
	DetectCollision->SetDebugColor({ 1.0f, 1.0f, 0.0f });
}

void AMonster::ActivePixelCollision()
{
	if (true == IsCurRoom())
	{
		ParentRoom->CheckPixelCollisionWithGravity(this, BodyRenderer.get(), GravityPointOffset);
		   ParentRoom->CheckPixelCollisionWithWall(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft, WallPointOffest);
		   ParentRoom->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft, CeilPointOffset);
	}
}