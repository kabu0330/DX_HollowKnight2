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

}

void AKnightSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
	this;
	//CheckKnightPos();
	//Release();
}

//void AKnightSkill::CheckKnightPos()
//{
//	//KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
//}




