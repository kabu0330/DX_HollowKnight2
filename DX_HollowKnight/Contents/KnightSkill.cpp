#include "PreCompile.h"
#include "KnightSkill.h"
#include "Skill.h"

AKnightSkill::AKnightSkill()
{
	float FrameTime = 0.03f;

	std::string SlashEffect = "SlashEffect";
	BodyRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, FrameTime, false);

	std::string UpSlashEffect = "UpSlashEffect";
	BodyRenderer->CreateAnimation(UpSlashEffect, UpSlashEffect, 0, 5, FrameTime, false);

	std::string DownSlashEffect = "DownSlashEffect";
	BodyRenderer->CreateAnimation(DownSlashEffect, DownSlashEffect, 0, 5, FrameTime, false);

	float FireballFrame = 0.1f;
	std::string Fireball = "Fireball";
	BodyRenderer->CreateAnimation(Fireball, Fireball, 0, 3, FireballFrame);

	std::string FireballImpact = "Fireball_Impact";
	BodyRenderer->CreateAnimation(FireballImpact, FireballImpact, 0, 5, FireballFrame);

	BodyRenderer->ChangeAnimation(SlashEffect);
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




