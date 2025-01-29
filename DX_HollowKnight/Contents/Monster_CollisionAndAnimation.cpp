#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"

void AMonster::CreateAnimation()
{
	// 재정의하여 사용
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
	//return;

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

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			Knockback();
		});

	int Att = Stat.GetAtt();
	UFightUnit::OnHit(OtherKnight, Att);

	int KnightCurHp = OtherKnight->GetStatRef().GetHp();
	UEngineDebug::OutPutString(GetName() + "의 공격으로 나이트가 " + std::to_string(Att) + "의 피해를 입었습니다. 남은 체력 : " + std::to_string(KnightCurHp));
}

void AMonster::Knockback()
{
	FVector Pos = { GetActorLocation().X, GetActorLocation().Y };
	FVector KnightPos = { Knight->GetActorLocation().X, Knight->GetActorLocation().Y };
	FVector KnockbackDirection = KnightPos - Pos;
	KnockbackDirection.Y = 0.0f;
	KnockbackDirection.Normalize();
	KnockbackDirection += FVector::UP;
	KnockbackDirection.Normalize();

	Knight->GetStatRef().SetKnockbackDistance(500.0f);
	Knight->GetStatRef().SetKnockbackDir(KnockbackDirection);
}

void AMonster::CreateCollision()
{
	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetScale3D(CollisionScale);
	BodyCollision->SetWorldLocation({ BodyCollisionOffset.X, BodyCollisionOffset.Y, ZSort });
	BodyCollision->SetCollisionProfileName("Monster");
}

void AMonster::CreateAttackEffect()
{
}

void AMonster::ResetRendererOffest()
{
	BodyRenderer->SetRelativeLocation({ RendererOffset.X, RendererOffset.Y, ZSort });
}

void AMonster::SetWalkRendererOffset()
{
	if (FVector::ZERO == WalkRendererOffset)
	{
		return;
	}
	BodyRenderer->SetRelativeLocation({ WalkRendererOffset.X, WalkRendererOffset.Y, ZSort });
}

void AMonster::SetAttackRendererOffset()
{
	if (FVector::ZERO == AttackRendererOffset)
	{
		return;
	}
	BodyRenderer->SetRelativeLocation({ AttackRendererOffset.X, AttackRendererOffset.Y, ZSort });
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
	PixelCollision = CreateDefaultSubObject<UCollision>();
	PixelCollision->SetupAttachment(RootComponent);
	PixelCollision->SetCollisionProfileName("MonsterPoint");
	PixelCollision->SetScale3D({ 100, 5 });
	float Offset = ::abs(BodyCollision->GetWorldScale3D().Half().Y);
	PixelCollision->SetRelativeLocation({ 0.0f + BodyCollisionOffset.X, -Offset + BodyCollisionOffset.Y});
	PixelCollision->SetDebugColor({ 1.0f, 1.0f, 1.0f });
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

