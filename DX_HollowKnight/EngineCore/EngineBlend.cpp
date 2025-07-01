#include "PreCompile.h"
#include "EngineBlend.h"
#include "EngineCore.h"

UEngineBlend::UEngineBlend()
{
}

UEngineBlend::~UEngineBlend()
{
}

std::shared_ptr<UEngineBlend> UEngineBlend::Create(std::string_view _Name, const D3D11_BLEND_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ؽ�ó�� �� �ε��Ϸ��� �߽��ϴ�." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineBlend> NewRes = std::make_shared<UEngineBlend>();
	PushResource<UEngineBlend>(NewRes, _Name, "");
	NewRes->CreateBlendState(_Value);

	return NewRes;
}

void UEngineBlend::CreateBlendState(const D3D11_BLEND_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateBlendState(&_Value, &BlendState))
	{
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
}

void UEngineBlend::OMSetBlendState()
{
	UEngineCore::GetDevice().GetContext()->OMSetBlendState(BlendState.Get(), BlendFactor.Arr1D, Mask);
}
