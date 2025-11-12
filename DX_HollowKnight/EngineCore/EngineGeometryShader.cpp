#include "PreCompile.h"
#include "EngineGeometryShader.h"
#include "EngineGraphicDevice.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

UEngineGeometryShader::UEngineGeometryShader()
{
	ShaderType = EShaderType::GS;
}

UEngineGeometryShader::~UEngineGeometryShader()
{
}

std::shared_ptr<UEngineGeometryShader> UEngineGeometryShader::LoadGeometryShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh, UINT _VersionLow)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == UEngineResourceBase::IsRes<UEngineGeometryShader>(UpperName))
	{
		return UEngineResourceBase::Find<UEngineGeometryShader>(UpperName);
	}

	std::shared_ptr<UEngineGeometryShader> NewRes = UEngineResourceBase::CreateRes<UEngineGeometryShader>(UpperName);

	UEnginePath Path = _Path;
	if (false == Path.IsExists())
	{
		MSGASSERT(Path.GetPathToString() + "셰이더 경로가 존재하지 않습니다.");
		return nullptr;
	}

	std::wstring WPath = UEngineString::AnsiToUnicode(Path.GetPathToString());

	std::string version = "gs_";
	version += std::to_string(_VersionHigh) + "_" + std::to_string(_VersionLow);

	std::string EntryName = _EntryPoint.data();
	NewRes->EntryName = EntryName;
	NewRes->VersionHigh = _VersionHigh;
	NewRes->VersionLow = _VersionLow;

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	// 셰이더 컴파일
	D3DCompileFromFile(
		WPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		EntryName.c_str(),
		version.c_str(),
		Flag0,
		Flag1,
		&NewRes->ShaderCodeBlob,
		&NewRes->ErrorCodeBlob
	);

	if (nullptr == NewRes->ShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(NewRes->ErrorCodeBlob->GetBufferPointer());
		MSGASSERT("지오메트리 셰이더 컴파일 에러\n" + std::string(ErrString.c_str()));
		return nullptr;
	}

	NewRes->CreateGeometryShader();

	NewRes->ReflectAndBindShaderResources();

	return NewRes;
}

void UEngineGeometryShader::CreateGeometryShader()
{
	if (FAILED(UEngineCore::GetDevice().GetDevice()->CreateGeometryShader(
		ShaderCodeBlob->GetBufferPointer(),
		ShaderCodeBlob->GetBufferSize(),
		nullptr,
		&GeometryShader)))
	{
		MSGASSERT("지오메트리 셰이더 생성 실패");
	}
}

void UEngineGeometryShader::GSSetShader()
{
	UEngineCore::GetDevice().GetContext()->GSSetShader(GeometryShader.Get(), nullptr, 0);
}

void UEngineGeometryShader::GSUnsetShader()
{
	UEngineCore::GetDevice().GetContext()->GSSetShader(nullptr, nullptr, 0);
}
