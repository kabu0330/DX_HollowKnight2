#include "PreCompile.h"
#include "EngineSampler.h"
#include "EngineCore.h"

UEngineSampler::UEngineSampler()
{
}

UEngineSampler::~UEngineSampler()
{
}

std::shared_ptr<UEngineSampler> UEngineSampler::LoadSampler(std::string_view _Name, const D3D11_SAMPLER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 등록된 샘플러입니다. \n" + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineSampler> NewRes = std::make_shared<UEngineSampler>();
	PushResource<UEngineSampler>(NewRes, _Name, "");
	NewRes->CreateSamplerState(_Value);

	return NewRes;
}

void UEngineSampler::CreateSamplerState(const D3D11_SAMPLER_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&_Value, &SamplerState))
	{
		MSGASSERT("블랜드 스테이트 생성에 실패했습니다");
		return;
	}
	
}

void UEngineSampler::BindToSamplerSlot(EShaderType _Type, UINT _BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { SamplerState.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("아직 존재하지 않는 셰이더에 바인드할 수 없습니다.");
		break;
	}
}

void UEngineSampler::UnbindFromSamplerSlot(EShaderType _Type, UINT _BindIndex)
{
	ID3D11SamplerState* ArrPtr[1] = { nullptr };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetSamplers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("아직 존재하지 않는 셰이더에 바인드할 수 없습니다.");
		break;
	}
}
