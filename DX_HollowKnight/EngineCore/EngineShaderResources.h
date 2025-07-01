#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

// ���� : ���̴� ���ҽ��� �̸��� ���̴� Ÿ��(VS, PS ��) �׸��� ���° �ε��������� �ϳ��� �����ͷ� �����Ѵ�.
class UEngineShaderResourceBase 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX_ShaderType;
	UINT BindIndex = 0;
};

// ���� : ������� ���ҽ��� ���̴� ���ҽ� + ���� ������ + ��� ����
class UEngineConstantBufferResource : public UEngineShaderResourceBase
{
public:
	void* Data = nullptr; // �ڽſ��� ���õ� �����ʹ� ������ ������ ���� ���̴�.
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> ConstantBuffer;

	void BindToShaderSlot()
	{
		if (nullptr != Data)
		{
			Name;
			ConstantBuffer->UpdateConstantBufferData(Data, BufferSize);
		}

		ConstantBuffer->BindToShaderSlot(ShaderType, BindIndex);
	}

	void UnbindFromShaderSlot()
	{
	}
};

class UEngineTextureResource : public UEngineShaderResourceBase
{
public:
	std::shared_ptr<UEngineTexture> Texture;

	void BindToShaderResources()
	{
		Texture->BindToShaderResources(ShaderType, BindIndex);
	}

	void UnbindFromShaderResources()
	{
		Texture->UnbindFromShaderResources(ShaderType, BindIndex);
	}
};

class UEngineSamplerResource : public UEngineShaderResourceBase
{
public:
	std::shared_ptr<UEngineSampler> Sampler;

	void BindToSamplerSlot()
	{
		Sampler->BindToSamplerSlot(ShaderType, BindIndex);
	}

	void UnbindFromSamplerSlot()
	{
		Sampler->UnbindFromSamplerSlot(ShaderType, BindIndex);
	}
};

// ���� : 
class UEngineShaderResources
{
public:
	UEngineShaderResources();
	~UEngineShaderResources();

	void PushSamplerResource(std::string_view _Name, UEngineSamplerResource _Res);

	void PushTextureResource(std::string_view _Name, UEngineTextureResource _Res);

	void PushConstantBufferResource(std::string_view _Name, UEngineConstantBufferResource _Res);

	template<typename DataType>
	void LinkConstantBufferData(std::string_view _Name, DataType& _Data)
	{
		LinkConstantBufferData(_Name, reinterpret_cast<void*>(&_Data));
	}

	void LinkConstantBufferData(std::string_view _Name, void* _Data);

	void PushSampler(std::string_view _Name, std::string_view _ResName);
	void PushTexture(std::string_view _Name, std::string_view _ResName);
	void PushTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);

	bool HasSampler(std::string_view _Name);
	bool HasTexture(std::string_view _Name);
	bool HasConstantBuffer(std::string_view _Name);
	void BindToShaderSlot();

	void UnbindFromShaderSlot();

protected:

private:
	std::map<std::string, UEngineConstantBufferResource> AllConstantBufferResources;
	std::map<std::string, UEngineTextureResource> AllTextureResources;
	std::map<std::string, UEngineSamplerResource> AllSamplerResources;
};

