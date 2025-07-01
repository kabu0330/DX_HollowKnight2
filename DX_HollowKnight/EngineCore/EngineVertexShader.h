#pragma once
#include "EngineResourceBase.h"
#include <Windows.h>
#include "EngineShaderBase.h"

// 설명 : 셰이더 파일로부터 버텍스 셰이더를 컴파일하고 바이트 코드와 리플렉션 정보를 포함하여
//		  렌더링 파이프라인의 VS 슬롯에 바인딩하는 클래스
class UEngineVertexShader : public UEngineResourceBase, public UEngineShaderBase
{
public:
	UEngineVertexShader();
	~UEngineVertexShader();

	// 버텍스 셰이더를 생성하고 DX에 등록하는 함수
	static std::shared_ptr<UEngineVertexShader> LoadVertexShader(std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileNameToString();

		return LoadVertexShader(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}

	ENGINEAPI static std::shared_ptr<UEngineVertexShader> LoadVertexShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

	void VSSetShader();

protected:

private:
	ENGINEAPI void CreateVertexShader();

	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader = nullptr;

private:
	// delete Function
	UEngineVertexShader(const UEngineVertexShader& _Other) = delete;
	UEngineVertexShader(UEngineVertexShader&& _Other) noexcept = delete;
	UEngineVertexShader& operator=(const UEngineVertexShader& _Other) = delete;
	UEngineVertexShader& operator=(UEngineVertexShader&& _Other) noexcept = delete;
};

