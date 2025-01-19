#include "PreCompile.h"
#include "Skill.h"
#include <EnginePlatform/EngineInput.h>

ASkill::ASkill()
{
	SetName("ASkill");

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("KnightObject");
	ZSort = static_cast<float>(EZOrder::KNIGHT_SKILL_FRONT);
	Collision->GetTransformRef().Location.Z = ZSort;
	//Collision->SetCollisionType(ECollisionType::AABB);

	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
}

void ASkill::BeginPlay()
{
	AEffect::BeginPlay();
	SetCollisionEvent();

	CollisionScale = BodyRenderer->GetScale();
	Collision->SetScale3D(CollisionScale);
}

void ASkill::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
	Release();
}

void ASkill::Release()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		if (nullptr != Collision)
		{
			UEngineDebug::OutPutString(GetName() + " Skill Collision Destroy");
			Collision->Destroy();
		}
		Destroy();
	}
}

void ASkill::SetCollisionEvent()
{
	Collision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			UEngineDebug::OutPutString("나이트의 공격 적중");
		});
}

ASkill::~ASkill()
{
}

