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
	std::shared_ptr<AKnightEffect> Effect2 = GetWorld()->SpawnActor<AKnightEffect>();
	Effect2->ChangeAnimation("Dash_effect2",GetActorLocation());
	Effect2->SetZSort(static_cast<int>(EZOrder::KNIGHT_SKILL_BACK) + 1);
	Effect2->SetLocation(this, {200.0f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f});
}

void AKnight::CreateStunEffect()
{
	if (true == bIsStunEffect)
	{
		return;
	}

	BodyRenderer->AddPlusColor({ -0.5f, -0.5f, -0.5f });

	std::shared_ptr<KnightStunEffect> Effect = GetWorld()->SpawnActor<KnightStunEffect>();
	Effect->ChangeAnimation("StunEffect01", GetActorLocation());
	Effect->SetScale(2.0f);
	FVector Offest = { 0.0f, 0.0f, 0.0f };
	UEngineRandom Random;
	float Degree = Random.Randomfloat(0.0f, 30.0f);
	FVector Rotation = { 0.0f, 0.0f, Degree };
	Effect->SetLocation(this, Offest, Rotation);
	TimeEventer->AddEndEvent(0.3f, std::bind(&KnightStunEffect::CreateStunImpactEffect, Effect));
	bIsStunEffect = true;
}


void AKnight::CreateFocusEffect()
{
	if (true == bIsFocusEffect)
	{
		return;
	}
	std::shared_ptr<AKnightEffect> FocusEffect = GetWorld()->SpawnActor<AKnightEffect>();
	FocusEffect->ChangeAnimation("FocusEffect", GetActorLocation());
	FVector Offest = { 5.0f, 30.0f, 0.0f };
	FocusEffect->SetLocation(this, Offest);
	FocusEffect->EnableRotation(false); // 좌우반전에 따라 
	FocusEffect->GetRenderer()->SetAutoScaleRatio(1.5f);

	FocusEffect->GetRenderer()->SetMulColor({ 12.0f, 12.0f, 12.0f }, 0.1f);


	bIsFocusEffect = true;
}

void AKnight::CreateFocusEndEffect()
{
	if (true == bIsFocusEndEffect)
	{
		return;
	}
	FVector Offset = { 10.0f, 10.0f, 0.0f };

	std::shared_ptr<AKnightEffect> BurstEffect = GetWorld()->SpawnActor<AKnightEffect>();
	BurstEffect->ChangeAnimation("BurstEffect", GetActorLocation());
	BurstEffect->SetLocation(this, { Offset.X, Offset.Y });
	BurstEffect->EnableRotation(false); // 좌우반전에 따라 
	BurstEffect->GetRenderer()->SetAutoScaleRatio(1.6f);

	BurstEffect->GetRenderer()->SetMulColor({ 11.0f, 11.0f, 11.0f }, 0.1f);

	Stat.AddHp(1);
	Stat.AddMp(-33);

	bIsFocusEndEffect = true;
}

void AKnight::CreateFireballEffect()
{
	if (true == bIsFireballEffect)
	{
		return;
	}
	std::shared_ptr<AKnightEffect> Effect = GetWorld()->SpawnActor<AKnightEffect>();
	Effect->ChangeAnimation("Blast", GetActorLocation());
	FVector Offset = { -80.0f, 0.0f, 0.0f };
	Effect->SetLocation(this, Offset);
	//Effect->EnableRotation(false); // 좌우반전에 따라 
	Effect->ToggleFlip(); // 좌우반전
	Effect->GetRenderer()->SetAutoScaleRatio(1.5f);

	Effect->GetRenderer()->SetMulColor({ 11.0f, 11.0f, 11.0f }, 0.1f);

	std::shared_ptr<AKnightFireball> Skill = GetWorld()->SpawnActor<AKnightFireball>();
	Skill->ChangeAnimation("Fireball");
	Skill->SetParentRoom(ARoom::GetCurRoom()); // 픽셀 충돌 검사
	FVector Speed = { 2000.0f, 0.0f,0.0f };
	Skill->SetZSort(static_cast<int>(Effect->GetZSort() - 5.0f));
	Skill->AddLocation(this, Speed);
	Skill->ToggleFlip(); // 좌우반전

	Skill->GetRenderer()->SetMulColor({ 11.0f, 11.0f, 11.0f }, 0.1f);

	bIsFireballEffect = true;
}

void AKnight::CreateSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}
	bIsShowEffect = true;

	std::shared_ptr<AKnightSlash> SlashEffect = GetWorld()->SpawnActor<AKnightSlash>();
	SlashEffect->ChangeAnimation("SlashEffect", GetActorLocation());
	FVector Offset = FVector{ -120.0f, 0.0f };
	SlashEffect->SetLocation(this, Offset);


	return;
}

void AKnight::CreateUpSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSlash> SlashEffect = GetWorld()->SpawnActor<AKnightSlash>();
	SlashEffect->ChangeAnimation("UpSlashEffect", GetActorLocation());
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
	SlashEffect->ChangeAnimation("DownSlashEffect", GetActorLocation());
	FVector Offset = FVector{ 0.0f, -100.0f };
	SlashEffect->SetLocation(this, Offset);
	SlashEffect->EnableRotation(false);
	bIsShowEffect = true;
}