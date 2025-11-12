#pragma once
#include "EngineResourceBase.h"
#include <Windows.h>
#include "EngineShaderBase.h"

// 설명 : 지오메트리 셰이더를 로드하고 바인딩하는 클래스
class UEngineGeometryShader : public UEngineResourceBase, public UEngineShaderBase
{
public:
	UEngineGeometryShader();
	~UEngineGeometryShader();

	// 지오메트리 셰이더를 로드하고 컴파일하는 함수
	static std::shared_ptr<UEngineGeometryShader> LoadGeometryShader(std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileNameToString();

		return LoadGeometryShader(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}

	ENGINEAPI static std::shared_ptr<UEngineGeometryShader> LoadGeometryShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

	void GSSetShader();
	void GSUnsetShader();

protected:

private:
	ENGINEAPI void CreateGeometryShader();

	Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader = nullptr;

private:
	// delete Function
	UEngineGeometryShader(const UEngineGeometryShader& _Other) = delete;
	UEngineGeometryShader(UEngineGeometryShader&& _Other) noexcept = delete;
	UEngineGeometryShader& operator=(const UEngineGeometryShader& _Other) = delete;
	UEngineGeometryShader& operator=(UEngineGeometryShader&& _Other) noexcept = delete;
};
