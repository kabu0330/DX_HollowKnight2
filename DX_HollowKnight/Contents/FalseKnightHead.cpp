#include "PreCompile.h"
#include "FalseKnightHead.h"

AFalseKnightHead::AFalseKnightHead()
{
	SetName("MonsterSkill");
	Collision->SetCollisionProfileName("MonsterHead");

	// 이펙트 없이 컬리전만
	BodyRenderer->SetName("FalseKnight Head");
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	FVector HeadScale = { 120.0f, 120.0f };
	//BodyRenderer->SetAutoScale(false);
	//BodyRenderer->SetRelativeScale3D(HeadScale);

	std::string AFalseKnightHead = "FalseKnightHead.png";
	std::string FalseKnightDeath = "FalseKnightDeath.png";

	float FrameTime = 0.5f;
	BodyRenderer->CreateAnimation("Stun", AFalseKnightHead, 0, 4, FrameTime);
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

void AFalseKnightHead::Release()
{
}

