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

	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 12, FrameTime, false);

	std::string FocusEffectEnd = "FocusEffectEnd";
	BodyRenderer->CreateAnimation(FocusEffectEnd, FocusEffectEnd, 0, 1, FrameTime, false);

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




