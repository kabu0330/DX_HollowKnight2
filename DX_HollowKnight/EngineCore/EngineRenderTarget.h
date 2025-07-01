#pragma once
#include "EngineResourceBase.h"
#include "EngineTexture.h"
#include "RenderUnit.h"
#include "EngineGraphicDevice.h"

// ���� : ���� Ÿ�� �� ����, ���̹��� ����, ��� ���� �� ������ �������������� ����
class UEngineRenderTarget : public UEngineResourceBase
{
public:
	UEngineRenderTarget();
	~UEngineRenderTarget();

	ENGINEAPI void CreateRenderTargetView(float4 _Scale, float4 _ClearColor = float4::NONE, DXGI_FORMAT _Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	ENGINEAPI void CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D);

	ENGINEAPI void CreateDepthTexture(int _Index = 0);

	ENGINEAPI void ClearRenderTargets();
	ENGINEAPI void OMSetRenderTargets();

	ENGINEAPI void SetClearColor(float4 _ClearColor)
	{
		ClearColor = _ClearColor;
	}

	ENGINEAPI void CopyToRenderTarget(std::shared_ptr<UEngineRenderTarget> _Target);
	ENGINEAPI void MergeToRenderTarget(std::shared_ptr<UEngineRenderTarget> _Target);

protected:

private:
	float4 ClearColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<std::shared_ptr<class UEngineTexture>> AllTextures;
	std::vector<ID3D11RenderTargetView*> AllRenderTargetViews;
	std::vector<ID3D11ShaderResourceView*> AllShaderResourceViews;

	std::shared_ptr<class UEngineTexture> DepthTexture;

	URenderUnit TargetUnit;

private:
	// delete Function
	UEngineRenderTarget(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget(UEngineRenderTarget&& _Other) noexcept = delete;
	UEngineRenderTarget& operator=(const UEngineRenderTarget& _Other) = delete;
	UEngineRenderTarget& operator=(UEngineRenderTarget&& _Other) noexcept = delete;
};

