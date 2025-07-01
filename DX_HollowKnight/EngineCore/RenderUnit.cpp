#include "PreCompile.h"
#include "RenderUnit.h"
#include "EngineEnums.h"
#include "Renderer.h"

URenderUnit::URenderUnit()
{
}

URenderUnit::~URenderUnit()
{
}

void URenderUnit::CheckMaterialResources()
{
	if (nullptr == Material)
	{
		MSGASSERT("���� ������ ��Ƽ������ �������� �ʽ��ϴ�.");
		return;
	}

	{
		UEngineShaderResources& VS = Material->GetVertexShader()->AllShaderResources;
		AllShaderResources[EShaderType::VS] = Material->GetVertexShader()->AllShaderResources;
	}

	{
		UEngineShaderResources& PS = Material->GetPixelShader()->AllShaderResources;
		AllShaderResources[EShaderType::PS] = Material->GetPixelShader()->AllShaderResources;
	}

	// �θ� �������� ���ٸ�, �װ� UI��.
	if (nullptr != ParentRenderer)
	{
		TransformObject = ParentRenderer; // UI�� Ʈ������ ������Ʈ�� ������ �ִ�.
	}

	if (nullptr != TransformObject)
	{
		for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
		{
			if (false == AllShaderResources.contains(i))
			{
				continue;
			}

			if (false == AllShaderResources[i].HasConstantBuffer("FTransform"))
			{
				continue;
			}

			FTransform& Transform = TransformObject->GetTransformRef();
			AllShaderResources[i].LinkConstantBufferData("FTransform", Transform);
		}	
	}
}

void URenderUnit::LinkConstantBufferData(std::string_view _Name, void* _Data)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == AllShaderResources.contains(i))
		{
			continue;
		}

		if (false == AllShaderResources[i].HasConstantBuffer(_Name))
		{
			continue;
		}

		AllShaderResources[i].LinkConstantBufferData(_Name, _Data);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == AllShaderResources.contains(i))
		{
			continue;
		}

		if (false == AllShaderResources[i].HasTexture(_Name))
		{
			continue;
		}

		AllShaderResources[i].PushTexture(_Name, _ResName);
	}
}

void URenderUnit::SetTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == AllShaderResources.contains(i))
		{
			continue;
		}

		if (false == AllShaderResources[i].HasTexture(_Name))
		{
			continue;
		}

		AllShaderResources[i].PushTexture(_Name, _Texture);
	}
}

void URenderUnit::SetSampler(std::string_view _Name, std::string_view _ResName)
{
	for (EShaderType i = EShaderType::VS; i < EShaderType::MAX_ShaderType; i = static_cast<EShaderType>(static_cast<int>(i) + 1))
	{
		if (false == AllShaderResources.contains(i))
		{
			continue;
		}

		if (false == AllShaderResources[i].HasSampler(_Name))
		{
			continue;
		}

		AllShaderResources[i].PushSampler(_Name, _ResName);
	}
}

void URenderUnit::SetMesh(std::string_view _Name)
{
	Mesh = UMesh::Find<UMesh>(_Name);

	if (nullptr == Mesh)
	{
		MSGASSERT("�������� �ʴ� �Ž��� �����Ϸ��� �߽��ϴ�.");
	}

	if (nullptr != Material)
	{
		CreateInputLayout();
	}
}

void URenderUnit::SetMaterial(std::string_view _Name)
{
	Material = UEngineMaterial::Find<UEngineMaterial>(_Name);

	if (nullptr == Material)
	{
		MSGASSERT("�������� �ʴ� ��Ƽ�������� �����Ϸ��� �߽��ϴ�.");
	}

	CheckMaterialResources();


	if (nullptr != Mesh)
	{
		CreateInputLayout();
	}
}

void URenderUnit::Render(class UEngineCamera* _Camera, float _DeltaTime)
{
	for (std::pair<const EShaderType, UEngineShaderResources>& Pair : AllShaderResources)
	{
		Pair.second.BindToShaderSlot();
	}

	Mesh->GetVertexBuffer()->IASetVertexBuffers();
	Material->GetVertexShader()->VSSetShader();
	
	Mesh->GetIndexBuffer()->IASetIndexBuffer();
	Material->IASetPrimitiveTopology();
	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayout.Get());

	Material->GetRasterizerState()->RSSetState();

	Material->GetPixelShader()->PSSetShader();

	Material->GetBlend()->OMSetBlendState();
	Material->GetDepthStencilState()->OMSetDepthStencilState();

	UEngineCore::GetDevice().GetContext()->DrawIndexed(Mesh->GetIndexBuffer()->GetIndexCount(), 0, 0);
}

void URenderUnit::CreateInputLayout()
{
	Microsoft::WRL::ComPtr<ID3DBlob> Blob = Material->GetVertexShader()->GetShaderCodeBlob();

	UEngineInputLayoutInfo* InputLayoutInfo = Mesh->GetVertexBuffer()->GetInputLayoutInfo();

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
		&InputLayoutInfo->InputLayOutData[0],
		static_cast<unsigned int>(InputLayoutInfo->InputLayOutData.size()),
		Blob->GetBufferPointer(),
		Blob->GetBufferSize(),
		&InputLayout);
}

void URenderUnit::UnbindFromShaderSlot()
{
	for (std::pair<const EShaderType, UEngineShaderResources>& Pair : AllShaderResources)
	{
		Pair.second.UnbindFromShaderSlot();
	}
}