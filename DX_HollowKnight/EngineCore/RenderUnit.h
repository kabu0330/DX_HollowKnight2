#pragma once
#include "EngineShaderResources.h"
#include "Mesh.h"
#include "EngineMaterial.h"
#include "EngineEnums.h"

// ���� : �������� �ּҴ���
class URenderUnit
{
public:
	URenderUnit();
	~URenderUnit();

	ENGINEAPI void SetMesh(std::string_view _Name);
	ENGINEAPI void SetMaterial(std::string_view _Name);

	ENGINEAPI virtual void Render(class UEngineCamera* _Camera, float _DeltaTime);

	ENGINEAPI void CheckMaterialResources();

	template<typename Data>
	ENGINEAPI void LinkConstantBufferData(std::string_view _Name, Data& _Data)
	{
		LinkConstantBufferData(_Name, reinterpret_cast<void*>(&_Data));
	}
	ENGINEAPI void LinkConstantBufferData(std::string_view Name, void* _Data);

	ENGINEAPI void SetTexture(std::string_view _Name, std::string_view _ResName);
	ENGINEAPI void SetTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);
	ENGINEAPI void SetSampler(std::string_view Name, std::string_view _ResName);

	ENGINEAPI void UnbindFromShaderSlot();

	UTransformObject* TransformObject = nullptr;

	class URenderer* ParentRenderer = nullptr;

	std::shared_ptr<UMesh> Mesh; 				// �޽�(��ü) 
	std::shared_ptr<UEngineMaterial> Material; 	// ��Ƽ����(�Ǻ�)

	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
	std::map<EShaderType, UEngineShaderResources> AllShaderResources;

private:
	void CreateInputLayout();
};

