#include "PreCompile.h"
#include "EngineDepthStencilState.h"
#include "EngineCore.h"

UEngineDepthStencilState::UEngineDepthStencilState()
{
}

UEngineDepthStencilState::~UEngineDepthStencilState()
{
}

std::shared_ptr<UEngineDepthStencilState> UEngineDepthStencilState::Create(std::string_view _Name, const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� ���� ���ٽ� ������Ʈ�Դϴ�. \n" + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineDepthStencilState> NewRes = std::make_shared<UEngineDepthStencilState>();
	PushResource<UEngineDepthStencilState>(NewRes, _Name, "");
	NewRes->CreateDepthStencilState(_Value);

	return NewRes;
}


void UEngineDepthStencilState::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateDepthStencilState(&_Value, &DepthStencilState))
	{
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
	
}

void UEngineDepthStencilState::OMSetDepthStencilState()
{
	UEngineCore::GetDevice().GetContext()->OMSetDepthStencilState(DepthStencilState.Get(), 0);
}
