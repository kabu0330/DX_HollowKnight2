#include "PreCompile.h"
#include "EngineShaderResources.h"
#include "EngineBase/EngineDebug.h"

UEngineShaderResources::UEngineShaderResources()
{
}

UEngineShaderResources::~UEngineShaderResources()
{
}

void UEngineShaderResources::PushSamplerResource(std::string_view _Name, UEngineSamplerResource _Res)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == AllSamplerResources.contains(UpperName))
	{
		MSGASSERT("셰이더에 이미 등록된 샘플러입니다. \n" + UpperName);
		return;
	}

	AllSamplerResources[UpperName] = _Res;
}

void UEngineShaderResources::PushTextureResource(std::string_view _Name, UEngineTextureResource _Res)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == AllTextureResources.contains(UpperName))
	{
		MSGASSERT("셰이더에 이미 등록된 텍스처입니다. \n" + UpperName);
		return;
	}

	AllTextureResources[UpperName] = _Res;
}

void UEngineShaderResources::PushConstantBufferResource(std::string_view _Name, UEngineConstantBufferResource _Res)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == AllConstantBufferResources.contains(UpperName))
	{
		MSGASSERT("셰이더에 이미 등록된 상수 버퍼입니다. \n" + UpperName);
		return;
	}

	AllConstantBufferResources[UpperName] = _Res;
}

void UEngineShaderResources::BindToShaderSlot()
{
	for (std::pair<const std::string, UEngineConstantBufferResource>& Res : AllConstantBufferResources)
	{
		Res.second.BindToShaderSlot();
	}

	for (std::pair<const std::string, UEngineTextureResource>& Res : AllTextureResources)
	{
		Res.second.BindToShaderResources();
	}

	for (std::pair<const std::string, UEngineSamplerResource>& Res : AllSamplerResources)
	{
		Res.second.BindToSamplerSlot();
	}
}

bool UEngineShaderResources::HasSampler(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return AllSamplerResources.contains(UpperName);
}

bool UEngineShaderResources::HasTexture(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return AllTextureResources.contains(UpperName);
}
bool UEngineShaderResources::HasConstantBuffer(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return AllConstantBufferResources.contains(UpperName);
}

void UEngineShaderResources::LinkConstantBufferData(std::string_view _Name, void* _Data)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllConstantBufferResources.contains(UpperName))
	{
		UEngineDebug::OutputString("상수 버퍼를 먼저 생성해주세요. PushConstantBufferResource()" + UpperName);
		return;
	}

	AllConstantBufferResources[UpperName].Data = _Data;
}

void UEngineShaderResources::PushSampler(std::string_view _Name, std::string_view _ResName)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllSamplerResources.contains(UpperName))
	{
		UEngineDebug::OutputString("AllConstantBufferResources.contains " + UpperName);
		return;
	}

	AllSamplerResources[UpperName].Sampler = UEngineSampler::Find<UEngineSampler>(_ResName);

}

void UEngineShaderResources::PushTexture(std::string_view _Name, std::string_view _ResName)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllTextureResources.contains(UpperName))
	{
		UEngineDebug::OutputString("AllConstantBufferResources.contains " + UpperName);
		return;
	}

	AllTextureResources[UpperName].Texture = UEngineTexture::Find<UEngineTexture>(_ResName);
}

void UEngineShaderResources::PushTexture(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllTextureResources.contains(UpperName))
	{
		UEngineDebug::OutputString("AllTextureResources.contains " + UpperName);
		return;
	}

	AllTextureResources[UpperName].Texture = _Texture;
}

void UEngineShaderResources::UnbindFromShaderSlot()
{
	for (std::pair<const std::string, UEngineTextureResource>& Res : AllTextureResources)
	{
		Res.second.UnbindFromShaderResources();
	}

	for (std::pair<const std::string, UEngineSamplerResource>& Res : AllSamplerResources)
	{
		Res.second.UnbindFromSamplerSlot();
	}
}