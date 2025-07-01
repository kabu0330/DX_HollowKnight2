#pragma once
#include "EngineEnums.h"
#include <EngineBase/EngineFile.h>
#include "EngineShaderResources.h"

// 설명 : 셰이더 파일을 자동 컴파일하고, 상수 버퍼 / 텍스처 / 샘플러를 셰이더 리플렉션 기반으로 자동 추출
//		  바인딩 정보를 구성하는 셰이더 관리 클래스
class UEngineShaderBase
{
	friend class UEngineVertexShader;

public:
	UEngineShaderBase();
	~UEngineShaderBase();

	// 셰이더 파일 내의 _VS, _PS 네이밍 규칙으로 버텍스 셰이더와 픽셀 셰이더를 자동 분리 컴파일
	ENGINEAPI static void AutoCompileShaderByNaming(UEngineFile& _File);

	UEngineShaderResources ShaderResources;

	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderCodeBlob()
	{
		return ShaderCodeBlob;
	};

protected:
	EShaderType ShaderType = EShaderType::MAX_ShaderType;
	UINT VersionHigh = 5;
	UINT VersionLow = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // 중간 컴파일 결과물
	std::string EntryName;

	// 바인딩 정보(이름, 인덱스, 타입)를 등록하는 함수
	void ReflectAndBindShaderResources();

private:
	// delete Function
	UEngineShaderBase(const UEngineShaderBase& _Other) = delete;
	UEngineShaderBase(UEngineShaderBase&& _Other) noexcept = delete;
	UEngineShaderBase& operator=(const UEngineShaderBase& _Other) = delete;
	UEngineShaderBase& operator=(UEngineShaderBase&& _Other) noexcept = delete;
};

