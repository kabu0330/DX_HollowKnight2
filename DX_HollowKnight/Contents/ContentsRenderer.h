#pragma once
#include <EngineCore/Renderer.h>
#include <EngineBase/EngineDelegate.h>
#include <map>
#include "Global.h"

// 설명 :
class UContentsRenderer : public URenderer
{
public:
	class FrameAnimation
	{
	public:
		UEngineSprite* Sprite = nullptr;
		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		std::map<int, EngineDelegate> Events;

		int CurIndex = 0;
		int ResultIndex = 0;
		float CurTime = 0.0f;
		bool Loop = true;
		bool IsEnd = false;

		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
			IsEnd = false;
		}
	};

	ENGINEAPI UContentsRenderer();
	ENGINEAPI ~UContentsRenderer();

	int GetCurIndex()
	{
		return CurIndex;
	}

	int GetFrameIndex()
	{
		return CurAnimation->CurIndex;
	}

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, int _Start, int _End, float Time = 0.1f, bool _Loop = true);

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame, bool _Loop = true);

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, float _Frame, bool _Loop = true);

	// 내가 Idle인데 Idle 바꾸라고 했다. 
	ENGINEAPI void ChangeAnimation(std::string_view _AnimationName, bool _Force = false);

	ENGINEAPI void SetAnimationEvent(std::string_view _AnimationName, int _Frame, std::function<void()> _Function);

	ENGINEAPI FrameAnimation* FindAnimation(std::string_view _AnimationName);

	ENGINEAPI std::string GetCurSpriteName()
	{
		return Sprite->GetName();
	}

	ENGINEAPI void SetSprite(std::string_view _Name, UINT _CurIndex = 0);

	ENGINEAPI void SetTexture(std::string_view _Name, bool AutoScale = false, float _Ratio = 1.0f);

	bool IsCurAnimationEnd()
	{
		return CurAnimation->IsEnd;
	}

	void SetAnimationSpeed(float _Speed)
	{
		CurAnimationSpeed = _Speed;
	}

	void ResetAnimationSpeed()
	{
		CurAnimationSpeed = 1.0f;
	}

	void SetAutoScale(bool _Value)
	{
		IsAutoScale = _Value;
	}

	void SetAutoScaleRatio(float _Scale)
	{
		AutoScaleRatio = _Scale;
	}
	float GetAutoScaleRatio()
	{
		return AutoScaleRatio;
	}

	void ActivateBillboard()
	{
		IsBillboard = true;
	}

	void DeactivateBillboard()
	{
		IsBillboard = false;
	}

	void SetSprite(UEngineSprite* _Sprite);

	FResultColor ColorData;
	FUVValue UVValue;
	FSpriteData SpriteData;

	FVector GetScale()
	{
		return CurAnimation->Sprite->GetTexture()->GetTextureSize();
	}
	FVector GetRealScale()
	{
		return Sprite->GetSpriteScaleToReal(CurIndex);
	}

	FVector GetActorLocation() 
	{
		return Transform.RelativeLocation;
	}

	void SetZSort(float _ZValue)
	{
		Transform.WorldLocation.Z = _ZValue;
	}
	template<typename ZType>
	void SetZSort(ZType _ZValue)
	{
		SetZSort(static_cast<float>(_ZValue));
	}

	void AddPlusColor(FVector _PlusColor)
	{
		ColorData.PlusColor += _PlusColor;
	}
	void SetMulColor(FVector _RGB, float _W = 1.0f)
	{
		ColorData.MulColor = _RGB;
		ColorData.MulColor.W = _W;
	}
	void SetAlpha(float _Alpha = 1.0f)
	{
		ColorData.MulColor.W = _Alpha;
	}


protected:
	ENGINEAPI void Render(class UEngineCamera* _Camera, float _DeltaTime) override;
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void ComponentTick(float _DeltaTime) override;
	ENGINEAPI void CameraTransUpdate(UEngineCamera* _Camera) override;

private:
	bool IsBillboard = false;

	URenderUnit* MainUnit = nullptr;

	int CurIndex = 0;
	float CurAnimationSpeed = 1.0f;

	std::map<std::string, FrameAnimation> FrameAnimations;
	FrameAnimation* CurAnimation = nullptr; 
	UEngineSprite* Sprite = nullptr;
	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;


private:
	// delete Function
	UContentsRenderer(const UContentsRenderer& _Other) = delete;
	UContentsRenderer(UContentsRenderer&& _Other) noexcept = delete;
	UContentsRenderer& operator=(const UContentsRenderer& _Other) = delete;
	UContentsRenderer& operator=(UContentsRenderer&& _Other) noexcept = delete;
};

