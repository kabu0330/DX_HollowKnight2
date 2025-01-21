#include "PreCompile.h"
#include "KnightSkill.h"
#include "Skill.h"

AKnightSkill::AKnightSkill()
{
	SetName("AKnightSkill");
	//Collision->SetCollisionProfileName("KnightObject");
}

AKnightSkill::~AKnightSkill()
{
}

void AKnightSkill::BeginPlay()
{
	ASkill::BeginPlay();
	Knight = AKnight::GetPawn();
}

void AKnightSkill::Tick(float _DeltaTime)
{
	ASkill::Tick(_DeltaTime);
}







