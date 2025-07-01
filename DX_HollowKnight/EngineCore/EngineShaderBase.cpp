#include "PreCompile.h"
#include "EngineShaderBase.h"
#include "EngineVertexShader.h"
#include "EnginePixelShader.h"
#include "EngineConstantBuffer.h"

UEngineShaderBase::UEngineShaderBase()
{
}

UEngineShaderBase::~UEngineShaderBase()
{
}

void UEngineShaderBase::AutoCompileShaderByNaming(UEngineFile& _File)
{
	_File.FileOpen("rt");
	std::string ShaderCode = _File.GetAllFileText();
	
	{
		size_t EntryIndex = ShaderCode.find("_VS(");

		if (EntryIndex != std::string::npos)
		{
			{
				// 역순으로 찾아나가는 함수
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);

				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_VS";

				UEngineVertexShader::LoadVertexShader(_File.GetPathToString(), EntryName);
				
			}
		}
	}

	{
		size_t EntryIndex = ShaderCode.find("_PS(");

		if (EntryIndex != std::string::npos)
		{
			{
	
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);

				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_PS";

				UEnginePixelShader::LoadPixelShader(_File.GetPathToString(), EntryName);
			}
		}
	}
}

void UEngineShaderBase::ReflectAndBindShaderResources()
{
	if (nullptr == ShaderCodeBlob)
	{
		MSGASSERT("셰이더가 컴파일되지 않았습니다.");
		return;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> CompileInfo = nullptr;

	if (S_OK != D3DReflect(ShaderCodeBlob->GetBufferPointer(), ShaderCodeBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &CompileInfo))
	{
		MSGASSERT("리플렉션에 실패했습니다.");
		return;
	}

	D3D11_SHADER_DESC Info;
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;

	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		std::string Name = ResDesc.Name;
		std::string UpperName = UEngineString::ToUpper(Name);

		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* Info = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC BufferInfo = { 0 };
			Info->GetDesc(&BufferInfo);

			std::shared_ptr<UEngineConstantBuffer> Buffer = UEngineConstantBuffer::CreateOrFindBuffer(BufferInfo.Size, UpperName);

			UEngineConstantBufferRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Buffer;
			NewRes.BufferSize = BufferInfo.Size;

			ShaderResources.CreateConstantBufferRes(UpperName, NewRes);
			break;
		}
		case D3D_SIT_TEXTURE:
		{
			std::shared_ptr<UEngineTexture> Res = UEngineTexture::Find<UEngineTexture>("NSBase.png");

			UEngineTextureRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Res;

			ShaderResources.CreateTextureRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_SAMPLER:
		{
			std::shared_ptr<UEngineSampler> Res = UEngineSampler::Find<UEngineSampler>("WRapSampler");

			UEngineSamplerRes NewRes;
			NewRes.ShaderType = ShaderType;
			NewRes.Name = UpperName;
			NewRes.BindIndex = ResDesc.BindPoint;
			NewRes.Res = Res;

			ShaderResources.CreateSamplerRes(UpperName, NewRes);

			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			break;
		}
		case D3D_SIT_UAV_RWSTRUCTURED:
		{
			break;
		}
		default:
			break;
		}
	}
}



