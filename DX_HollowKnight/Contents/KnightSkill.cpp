#include "PreCompile.h"
#include "KnightSkill.h"
#include "Skill.h"

AKnightSkill::AKnightSkill()
{
	float FrameTime = 0.03f;

	float FireballFrame = 0.1f;
	std::string Fireball = "Fireball";
	BodyRenderer->CreateAnimation(Fireball, Fireball, 0, 3, FireballFrame);

	std::string FireballImpact = "Fireball_Impact";
	BodyRenderer->CreateAnimation(FireballImpact, FireballImpact, 0, 5, FireballFrame);

	BodyRenderer->ChangeAnimation(FireballImpact);
}

AKnightSkill::~AKnightSkill()
{
}

void AKnightSkill::BeginPlay()
{
	ASkill::BeginPlay();
	SetCollisionEvent();
}

void AKnightSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
}

void AKnightSkill::SetCollisionEvent()
{
	Collision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			//_Other->GetActor()->Destroy();
			// _Other->Destroy();
			UEngineDebug::OutPutString("Enter");
		});
}





