#include "PreCompile.h"
#include "EngineVertexShader.h"

UEngineVertexShader::UEngineVertexShader()
{
	ShaderType = EShaderType::VS;
}

UEngineVertexShader::~UEngineVertexShader()
{
}

std::shared_ptr<UEngineVertexShader> UEngineVertexShader::LoadVertexShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh /*= 5*/, UINT _VersionLow /*= 0*/)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 등록된 버텍스 셰이더입니다. \n" + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineVertexShader> NewRes = std::make_shared<UEngineVertexShader>();
	PushResource<UEngineVertexShader>(NewRes, _Name, _Path);
	NewRes->VersionHigh = _VersionHigh;
	NewRes->VersionLow = _VersionLow;
	NewRes->EntryName = _EntryPoint;
	NewRes->CreateVertexShader();

	return NewRes;
}

void UEngineVertexShader::CreateVertexShader()
{
	std::wstring WPath = UEngineString::AnsiToUnicode(GetPath().GetPathToString());

	std::string version = "vs_" + std::to_string(VersionHigh) + "_" + std::to_string(VersionLow);

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr, 
		nullptr,
		EntryName.c_str(),
		version.c_str(),
		Flag0,
		Flag1,
		&ShaderCodeBlob,
		&ErrorCodeBlob
	);

	if (nullptr == ShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(ErrorCodeBlob->GetBufferPointer());
		MSGASSERT("버텍스 셰이더 중간 빌드에 실패했습니다.\n" + ErrString);
		return;
	}

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateVertexShader(
		ShaderCodeBlob->GetBufferPointer(),
		ShaderCodeBlob->GetBufferSize(),
		nullptr,
		&VertexShader
	);

	if (S_OK != Result)
	{
		MSGASSERT("버텍스 셰이더 생성에 실패했습니다.");
	}

	UEngineShaderBase::ReflectAndBindShaderResources();
}

void UEngineVertexShader::VSSetShader()
{
	UEngineCore::GetDevice().GetContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}