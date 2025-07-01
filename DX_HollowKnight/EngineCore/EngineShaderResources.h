#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

// 설명 : 셰이더 리소스는 이름과 셰이더 타입(VS, PS 등) 그리고 몇번째 인덱스인지를 하나의 데이터로 저장한다.
class UEngineShaderResourceBase 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX_ShaderType;
	UINT BindIndex = 0;
};

// 설명 : 상수버퍼 리소스는 셰이더 리소스 + 버퍼 사이즈 + 상수 버퍼
class UEngineConstantBufferResource : public UEngineShaderResourceBase
{
public:
	void* Data = nullptr; // 자신에게 세팅될 데이터는 스스로 가지고 있을 것이다.
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

// 설명 : 
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

