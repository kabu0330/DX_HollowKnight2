#include "PreCompile.h"
#include "FogEffect.h"

AFogEffect::AFogEffect()
{
	SetName("Fog Effect");

	float FrameTime = 0.06f;
	std::string Fog = "fog.png";
	BodyRenderer->CreateAnimation(Fog, Fog, 0, 10, FrameTime, false);

	BodyRenderer->ChangeAnimation(Fog);
}

AFogEffect::~AFogEffect()
{
}

void AFogEffect::BeginPlay()
{
	AEffect::BeginPlay();
}

void AFogEffect::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
}

