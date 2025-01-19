#include "PreCompile.h"
#include "PlayHUD.h"

APlayHUD::APlayHUD()
{
}

APlayHUD::~APlayHUD()
{
}

void APlayHUD::BeginPlay()
{
	AHUD::BeginPlay();
}

void APlayHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
}

