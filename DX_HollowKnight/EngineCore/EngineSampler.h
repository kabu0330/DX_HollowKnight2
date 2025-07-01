#pragma once
#include "EngineResourceBase.h"
#include "EngineEnums.h"

// Ό³Έν :
class UEngineSampler : public UEngineResourceBase
{
public:
	UEngineSampler();
	~UEngineSampler();

	static std::shared_ptr<UEngineSampler> LoadSampler(std::string_view _Name, const D3D11_SAMPLER_DESC& _Value);

	void BindToSamplerSlot(EShaderType _Type, UINT _BindIndex);

	void UnbindFromSamplerSlot(EShaderType _Type, UINT _BindIndex);

protected:
	void CreateSamplerState(const D3D11_SAMPLER_DESC& _Value);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState = nullptr;

private:
	// delete Function
	UEngineSampler(const UEngineSampler& _Other) = delete;
	UEngineSampler(UEngineSampler&& _Other) noexcept = delete;
	UEngineSampler& operator=(const UEngineSampler& _Other) = delete;
	UEngineSampler& operator=(UEngineSampler&& _Other) noexcept = delete;
};

