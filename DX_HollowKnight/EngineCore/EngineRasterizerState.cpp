#include "PreCompile.h"
#include "EngineRasterizerState.h"

UEngineRasterizerState::UEngineRasterizerState()
{
}

UEngineRasterizerState::~UEngineRasterizerState()
{
}

std::shared_ptr<UEngineRasterizerState> UEngineRasterizerState::Create(std::string_view _Name, const D3D11_RASTERIZER_DESC& _Value)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("�̹� �ε��� �ؽ�ó�� �ٽ� �ε��Ϸ��� �߽��ϴ�." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineRasterizerState> NewRes = std::make_shared<UEngineRasterizerState>();
	PushResource<UEngineRasterizerState>(NewRes, _Name, "");
	NewRes->CreateRasterizerState(_Value);

	return NewRes;
}


void UEngineRasterizerState::CreateRasterizerState(const D3D11_RASTERIZER_DESC& _Value)
{
	if (UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&_Value, &RasterizerState))
	{
		MSGASSERT("���� ������Ʈ ������ �����߽��ϴ�");
		return;
	}
}

void UEngineRasterizerState::RSSetState()
{
	UEngineCore::GetDevice().GetContext()->RSSetState(RasterizerState.Get());
}
