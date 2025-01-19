#include "PreCompile.h"
#include "Knight.h"
#include <EngineBase/EngineRandom.h>
#include "Effect.h"
#include "Skill.h"
#include "KnightSkill.h"
#include "KnightEffect.h"
#include "KnightSlash.h"
#include "KnightFireball.h"
#include "KnightStunEffect.h"

void AKnight::CreateDashEffect()
{
	std::shared_ptr<AKnightEffect> Effect = GetWorld()->SpawnActor<AKnightEffect>();
	Effect->ChangeAnimation(this, "Dash_effect");
	Effect->SetScale(0.5f);
	//Effect->Destroy(0.1f);
	std::shared_ptr<AKnightEffect> Effect2 = GetWorld()->SpawnActor<AKnightEffect>();
	Effect2->ChangeAnimation(this, "Dash_effect2");
	Effect2->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_BACK) + 1);
	Effect2->SetLocation(this, {200.0f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f});
}

void AKnight::CreateStunEffect()
{
	if (true == bIsStunEffect)
	{
		return;
	}
	std::shared_ptr<KnightStunEffect> Effect = GetWorld()->SpawnActor<KnightStunEffect>();
	Effect->ChangeAnimation(this, "StunEffect01");
	Effect->SetScale(2.0f);
	FVector Offest = { 0.0f, 0.0f, 0.0f };
	UEngineRandom Random;
	float Degree = Random.Randomfloat(0.0f, 30.0f);
	FVector Rotation = { 0.0f, 0.0f, Degree };
	Effect->SetLocation(this, Offest, Rotation);
	TimeEventor->AddEndEvent(0.3f, std::bind(&KnightStunEffect::CreateStunImpactEffect, Effect));
	bIsStunEffect = true;
}


void AKnight::CreateFocusEffect()
{
	if (true == bIsFocusEffect)
	{
		return;
	}
	std::shared_ptr<AKnightEffect> FocusEffect = GetWorld()->SpawnActor<AKnightEffect>();
	FocusEffect->ChangeAnimation(this, "FocusEffect");
	FVector Offest = { 0.0f, 30.0f, 0.0f };
	FocusEffect->SetLocation(this, Offest);
	FocusEffect->EnableRotation(false); // 좌우반전에 따라 
	FocusEffect->GetRenderer()->SetAutoScaleRatio(1.3f);
	bIsFocusEffect = true;
}

void AKnight::CreateFocusEndEffect()
{
	if (true == bIsFocusEndEffect)
	{
		return;
	}
	std::shared_ptr<AKnightEffect> FocusEffect = GetWorld()->SpawnActor<AKnightEffect>();
	FocusEffect->ChangeAnimation(this, "FocusEffectEnd");
	FVector Offest = { 0.0f, 30.0f, 0.0f };
	FocusEffect->SetLocation(this, Offest);
	FocusEffect->EnableRotation(false); // 좌우반전에 따라 
	FocusEffect->GetRenderer()->SetAutoScaleRatio(1.0f);
	bIsFocusEndEffect = true;
}

void AKnight::CreateFireballEffect()
{
	if (true == bIsFireballEffect)
	{
		return;
	}
	std::shared_ptr<AKnightFireball> Skill = GetWorld()->SpawnActor<AKnightFireball>();
	Skill->ChangeAnimation("Fireball");
	FVector Speed = { 2000.0f, 0.0f,0.0f };
	Skill->AddLocation(this, Speed);
	Skill->ToggleFlip(); // 좌우반전
	bIsFireballEffect = true;
}

void AKnight::CreateSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSlash> SlashEffect = GetWorld()->SpawnActor<AKnightSlash>();
	//SlashEffect->GetCollision()->SetupAttachment(BodyCollision);
	SlashEffect->ChangeAnimation("SlashEffect");
	FVector Offset = FVector{ -100.0f, 0.0f };
	SlashEffect->SetLocation(this, Offset);
	bIsShowEffect = true;

	return;
}

void AKnight::CreateUpSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSlash> SlashEffect = GetWorld()->SpawnActor<AKnightSlash>();
	SlashEffect->ChangeAnimation("UpSlashEffect");
	FVector Offset = FVector{ 0.0f, 100.0f };
	SlashEffect->SetLocation(this, Offset);
	SlashEffect->EnableRotation(false); // 좌우반전에 따라 
	bIsShowEffect = true;
}

void AKnight::CreateDownSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSlash> SlashEffect = GetWorld()->SpawnActor<AKnightSlash>();
	SlashEffect->ChangeAnimation("DownSlashEffect");
	FVector Offset = FVector{ 0.0f, -100.0f };
	SlashEffect->SetLocation(this, Offset);
	SlashEffect->EnableRotation(false);
	bIsShowEffect = true;
}