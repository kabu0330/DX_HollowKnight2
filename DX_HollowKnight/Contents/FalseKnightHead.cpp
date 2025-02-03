#include "PreCompile.h"
#include "FalseKnightHead.h"
#include "FalseKnight.h"

AFalseKnightHead::AFalseKnightHead()
{
	SetName("MonsterSkill");
	Collision->SetCollisionProfileName("MonsterHead");

	// ����Ʈ ���� �ø�����
	BodyRenderer->SetName("FalseKnight Head");
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	FVector HeadScale = { 120.0f, 120.0f };
	//BodyRenderer->SetAutoScale(false);
	//BodyRenderer->SetRelativeScale3D(HeadScale);
	ZSort = static_cast<float>(EZOrder::MONSTER) - 1.0f;
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	std::string AFalseKnightHead = "FalseKnightHead.png";
	std::string FalseKnightDeath = "FalseKnightDeath.png";

	float StunFrame = 0.5f;
	float FrameTime = 0.1f;
	BodyRenderer->CreateAnimation("Stun", AFalseKnightHead, 0, 4, StunFrame);
	BodyRenderer->CreateAnimation("StunHit", AFalseKnightHead, 5, 7, FrameTime, false);

	BodyRenderer->ChangeAnimation("Stun");
	BodyRenderer->SetActive(true);

	Collision->SetRelativeScale3D(HeadScale);
}

AFalseKnightHead::~AFalseKnightHead()
{
}

void AFalseKnightHead::BeginPlay()
{
	ASkill::BeginPlay();
}

void AFalseKnightHead::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
	
}

void AFalseKnightHead::SetCollisionEvent()
{
	Collision->SetCollisionEnter(std::bind(&AFalseKnightHead::Collide, this, std::placeholders::_1, std::placeholders::_2));
}

void AFalseKnightHead::Collide(UCollision* _This, UCollision* _Other)
{
	Attack();
}

void AFalseKnightHead::Attack()
{
	UEngineDebug::OutPutString("��� ����!!!");

	if (nullptr == Boss)
	{
		MSGASSERT("������ ����� �����͸� ������ ���� �ʽ��ϴ�. SetParent �Լ��� �̿��Ͽ� �����Ͽ� �ּ���.");
		return;
	}
	
	int Att = Knight->GetStatRef().GetAtt();
	Boss->AddHeadHp(-Att);

	if (false == bIsDamage)
	{
		Boss->GetFSM().ChangeState(EMonsterState::STUN_HIT);
		bIsDamage = true;
	}

}

void AFalseKnightHead::Release()
{
	if (false == bCanRelease)
	{
		return;
	}
	if (nullptr != Collision)
	{
		Collision->Destroy();
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		BodyRenderer->Destroy();
		Destroy();
	}
}

