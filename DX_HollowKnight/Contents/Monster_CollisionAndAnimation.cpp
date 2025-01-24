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
	// 재정의하여 사용
}

void AMonster::CreateAttackEffect()
{
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
	float Offset = BodyCollision->GetWorldScale3D().Half().Y;
	PixelCollision->SetRelativeLocation({ 0.0f, Offset });
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

