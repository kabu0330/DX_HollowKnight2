#include "PreCompile.h"
#include "TitleHUD.h"
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>

ATitleHUD::ATitleHUD()
{
}

ATitleHUD::~ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
{
	AHUD::BeginPlay();

}

void ATitleHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
}

