#include "PreCompile.h"
#include "Knight.h"
#include "Effect.h"
#include "Skill.h"
#include "KnightSkill.h"
#include "KnightSlash.h"
#include "KnightFireball.h"

void AKnight::CreateDashEffect()
{
	std::shared_ptr<AEffect> Effect = GetWorld()->SpawnActor<AEffect>();
	Effect->ChangeAnimation(this, "Dash_effect");
	Effect->SetScale(0.5f);
	//Effect->Destroy(0.1f);
	std::shared_ptr<AEffect> Effect2 = GetWorld()->SpawnActor<AEffect>();
	Effect2->ChangeAnimation(this, "Dash_effect2");
	Effect2->SetZSort(static_cast<float>(EZOrder::SKILL_BACK) + 1.0f);
	Effect2->SetLocation(this, {200.0f, 0.0f, 0.0f}, {0.0f, 180.0f, 0.0f});
	//Effect2->Destroy(0.1f);
	//Effect2->SetScale(0.5f);
	//FVector Offset = FVector{ -100.0f, 0.0f };
	//Effect->SetLocation(this, Offset);
}

void AKnight::CreateFocusEffect()
{
	if (true == bIsFocusEffect)
	{
		return;
	}
	std::shared_ptr<AEffect> FocusEffect = GetWorld()->SpawnActor<AEffect>();
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
	std::shared_ptr<AEffect> FocusEffect = GetWorld()->SpawnActor<AEffect>();
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
	std::shared_ptr<AKnightFireball> Effect = GetWorld()->SpawnActor<AKnightFireball>();
	Effect->ChangeAnimation("Fireball");
	FVector Speed = { 2000.0f, 0.0f,0.0f };
	Effect->AddLocation(this, Speed);
	Effect->ToggleFlip(); // 좌우반전
	bIsFireballEffect = true;
}

void AKnight::CreateSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSlash> SlashEffect = GetWorld()->SpawnActor<AKnightSlash>();
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