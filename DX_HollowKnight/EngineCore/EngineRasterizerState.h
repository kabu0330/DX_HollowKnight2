#pragma once
#include "EngineResourceBase.h"

// ���� :
class UEngineRasterizerState : public UEngineResourceBase
{
public:
	UEngineRasterizerState();
	~UEngineRasterizerState();

	static std::shared_ptr<UEngineRasterizerState> Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value);

	void RSSetState();

protected:

private:
	void CreateRasterizerState(const D3D11_RASTERIZER_DESC& _Value);
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState = nullptr;

private:
	// delete Function
	UEngineRasterizerState(const UEngineRasterizerState& _Other) = delete;
	UEngineRasterizerState(UEngineRasterizerState&& _Other) noexcept = delete;
	UEngineRasterizerState& operator=(const UEngineRasterizerState& _Other) = delete;
	UEngineRasterizerState& operator=(UEngineRasterizerState&& _Other) noexcept = delete;
};

