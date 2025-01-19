#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	SetName("Knight Effect");

	float FrameTime = 0.06f;
	float DashFrameTime = 0.03f;
	float DashFrameTime2 = 0.02f;

	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 10, FrameTime, false);

	std::string FocusEffectEnd = "FocusEffectEnd";
	BodyRenderer->CreateAnimation(FocusEffectEnd, FocusEffectEnd, 0, 1, FrameTime, false);

	std::string DashEffect = "Dash_effect";
	BodyRenderer->CreateAnimation(DashEffect, DashEffect, 0, 5, DashFrameTime, false);

	std::string DashEffect2 = "Dash_effect2";
	BodyRenderer->CreateAnimation(DashEffect2, DashEffect2, 0, 7, DashFrameTime2, false);

	std::string NailHitEffect = "NailHitEffect";
	BodyRenderer->CreateAnimation(NailHitEffect, NailHitEffect, 0, 4, FrameTime, false);

	std::string FireballImpact = "FireballImpact";
	BodyRenderer->CreateAnimation(FireballImpact, FireballImpact, 0, 5, FrameTime, false);

	std::string StunEffect02 = "StunEffect02";
	BodyRenderer->CreateAnimation(StunEffect02, StunEffect02, 0, 6, FrameTime, false);

	std::string StunEffect03 = "StunEffect03";
	BodyRenderer->CreateAnimation(StunEffect03, StunEffect03, 0, 7, FrameTime, false);


	BodyRenderer->ChangeAnimation(FocusEffect);
}

AKnightEffect::~AKnightEffect()
{
}

void AKnightEffect::BeginPlay()
{
	AEffect::BeginPlay();
}

void AKnightEffect::Tick(float _DeltaTime)
{
	AEffect::Tick(_DeltaTime);
}

