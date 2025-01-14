#include "PreCompile.h"
#include "TitleScene.h"
#include <EngineCore/EngineCore.h>

ATitleScene::ATitleScene()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	BackgroundRenderer->SetTexture("main.bmp", true, 0.67f);
	BackgroundRenderer->SetRelativeLocation({ 0.0f, 0.0f, 1000.0f });

	TitleLogo = CreateDefaultSubObject<UContentsRenderer>();
	TitleLogo->SetupAttachment(RootComponent);
	TitleLogo->SetTexture("TitleLogo.png", true, 0.6f);
	FVector ScreenSize = UEngineCore::GetScreenScale();
	TitleLogo->SetRelativeLocation({ 0.0f, ScreenSize.Y * 0.22f , 0.0f });

	CreditsLogo = CreateDefaultSubObject<UContentsRenderer>();
	CreditsLogo->SetupAttachment(RootComponent);
	CreditsLogo->SetTexture("creditslogo.png", true, 0.4f);
	CreditsLogo->SetRelativeLocation({ ScreenSize.X * 0.3f, ScreenSize.Y * -0.27f , 0.0f });


	//std::string TitleBackGround = "TitleBackGround";
	//BackgroundRenderer->CreateAnimation(TitleBackGround, TitleBackGround, 0, 111, 0.1f);
	//GlobalFunc::AutoScale(BackgroundRenderer, TitleBackGround);
	//BackgroundRenderer->ChangeAnimation(TitleBackGround);


	//Logo = GetWorld()->SpawnActor<ATitleLogo>();
//// Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
//Logo->GetRenderer()->SetSpriteData(4);
//
//// 카메라를 위치조정을 무조건 해줘야 한다.
//std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
//Camera->SetActorLocation({0.0f, 0.0f, -1000.0f, 1.0f});
}

ATitleScene::~ATitleScene()
{
}

void ATitleScene::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitleScene::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


}

