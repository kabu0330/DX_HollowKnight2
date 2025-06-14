#pragma once
#include "EngineResources.h"

// 설명 :
class UEngineBlend : public UEngineResources
{
public:
	UEngineBlend();
	~UEngineBlend();

	static std::shared_ptr<UEngineBlend> Create(std::string_view _Name, const D3D11_BLEND_DESC& _Value);

	void Setting();

protected:
	void ResCreate(const D3D11_BLEND_DESC& _Value);

private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> State = nullptr;
	FVector BlendFactor = FVector(0.0f, 0.0f, 0.0f, 0.0f);
	// 비트가 1인 부분만 
	UINT Mask = 0xFFFFFFFF;
	//            255 255 255 255
	//            R   G   B   A

private:
	// delete Function
	UEngineBlend(const UEngineBlend& _Other) = delete;
	UEngineBlend(UEngineBlend&& _Other) noexcept = delete;
	UEngineBlend& operator=(const UEngineBlend& _Other) = delete;
	UEngineBlend& operator=(UEngineBlend&& _Other) noexcept = delete;
};

