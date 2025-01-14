#pragma once
#include "TransformObject.h"
#include "EngineDataStruct.h"
#include "RenderUnit.h"

// 설명 : 랜더러를 겸한다고 생각하고
class UWidget : public UObject, public UTransformObject
{
	friend AHUD;

public:
	// constrcuter destructer
	ENGINEAPI UWidget();
	ENGINEAPI ~UWidget();

	URenderUnit& GetRenderUnit()
	{
		return RenderUnit;
	}

	// delete Function
	UWidget(const UWidget& _Other) = delete;
	UWidget(UWidget&& _Other) noexcept = delete;
	UWidget& operator=(const UWidget& _Other) = delete;
	UWidget& operator=(UWidget&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);
	void Render(class UEngineCamera* Camera, float _DeltaTime);

	ENGINEAPI void SetSprite(std::string_view _Name, UINT _Index = 0);
	ENGINEAPI void SetTexture(std::string_view _Name, bool AutoScale = false, float _Ratio = 1.0f);

	ULevel* GetWorld();

	void SetAutoScale(bool _Value)
	{
		IsAutoScale = _Value;
	}

	void SetAutoScaleRatio(float _Scale)
	{
		AutoScaleRatio = _Scale;
	}

	FResultColor ColorData;
	FUVValue UVValue;
	FSpriteData SpriteData;

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

	// 플레이스 먼트 new 할때 사용할 녀석이므로 초기화를 하면 안된다.
	class AHUD* HUD;

	int CurIndex = 0;
	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;
	class UEngineSprite* Sprite = nullptr;

	URenderUnit RenderUnit;
};

