#include "PreCompile.h"
#include "Skill.h"
#include <EnginePlatform/EngineInput.h>

ASkill::ASkill()
{
	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("KnightObject");
	ZSort = static_cast<float>(EZOrder::SKILL_FRONT);
	Collision->GetTransformRef().Location.Z = ZSort;
		//Collision->SetCollisionType(ECollisionType::AABB);

	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });


}

void ASkill::BeginPlay()
{
	AEffect::BeginPlay();

	CollisionScale = BodyRenderer->GetScale();
	Collision->SetScale3D(CollisionScale);

	Collision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			_Other->GetActor()->Destroy();
			// _Other->Destroy();
			UEngineDebug::OutPutString("Attack");
		});
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
			Collision->Destroy();
		}
		Destroy();
	}
}

ASkill::~ASkill()
{
}

