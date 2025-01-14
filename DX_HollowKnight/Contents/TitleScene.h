#pragma once
#include <EngineCore/Actor.h>

// ���� :
class ATitleScene : public AActor
{
public:
	// constrcuter destructer
	ATitleScene();
	~ATitleScene();

	// delete Function
	ATitleScene(const ATitleScene& _Other) = delete;
	ATitleScene(ATitleScene&& _Other) noexcept = delete;
	ATitleScene& operator=(const ATitleScene& _Other) = delete;
	ATitleScene& operator=(ATitleScene&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

protected:

private:
	std::shared_ptr<class UContentsRenderer> BackgroundRenderer;
	std::shared_ptr<class UContentsRenderer> TitleLogo;
	std::shared_ptr<class UContentsRenderer> CreditsLogo;
};

