#pragma once
#include "TransformObject.h"
#include "EngineDataStruct.h"
#include "RenderUnit.h"

// ���� :
class UWidget : public UObject, public UTransformObject
{
	friend AHUD;

public:
	ENGINEAPI UWidget();
	ENGINEAPI ~UWidget();

	virtual void Tick(float _DeltaTime);
	virtual void Render(class UEngineCamera* Camera, float _DeltaTime);

	class ULevel* GetWorld();


	void SetHoverEvent(std::function<void()> _Value)
	{
		Hover = _Value;
	}

	void SetDownEvent(std::function<void()> _Value)
	{
		Down = _Value;
	}

	void SetUpEvent(std::function<void()> _Value)
	{
		Up = _Value;
	}

private:
	std::function<void()> Hover;
	std::function<void()> Down;
	std::function<void()> Up;

	class AHUD* HUD;


private:
	// delete Function
	UWidget(const UWidget& _Other) = delete;
	UWidget(UWidget&& _Other) noexcept = delete;
	UWidget& operator=(const UWidget& _Other) = delete;
	UWidget& operator=(UWidget&& _Other) noexcept = delete;
};

