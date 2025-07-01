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
		MSGASSERT("�̹� ��ϵ� ���÷��Դϴ�. \n" + UpperName);
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
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
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
		MSGASSERT("���� �������� �ʴ� ���̴��� ���ε��� �� �����ϴ�.");
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
		MSGASSERT("���� �������� �ʴ� ���̴��� ���ε��� �� �����ϴ�.");
		break;
	}
}
