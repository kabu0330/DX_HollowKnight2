#pragma once
#include <EngineCore/HUD.h>

// Ό³Έν :
class AFade : public AHUD
{
public:
	// constrcuter destructer
	AFade();
	~AFade();

	// delete Function
	AFade(const AFade& _Other) = delete;
	AFade(AFade&& _Other) noexcept = delete;
	AFade& operator=(const AFade& _Other) = delete;
	AFade& operator=(AFade&& _Other) noexcept = delete;

	// 0 => 1
	void FadeIn();

	// 1 => 0
	void FadeOut();

	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

protected:

private:
	void FadeChange();

	float FadeValue = 0.0f;
	float FadeDir = 1.0f;
	class UImageWidget* Fade;
	class UTimeEventComponent* TimeEventor = nullptr;
};

