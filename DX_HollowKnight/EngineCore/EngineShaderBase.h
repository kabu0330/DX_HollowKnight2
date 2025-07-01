#pragma once
#include "EngineEnums.h"
#include <EngineBase/EngineFile.h>
#include "EngineShaderResources.h"

// ���� : ���̴� ������ �ڵ� �������ϰ�, ��� ���� / �ؽ�ó / ���÷��� ���̴� ���÷��� ������� �ڵ� ����
//		  ���ε� ������ �����ϴ� ���̴� ���� Ŭ����
class UEngineShaderBase
{
	friend class UEngineVertexShader;

public:
	UEngineShaderBase();
	~UEngineShaderBase();

	// ���̴� ���� ���� _VS, _PS ���̹� ��Ģ���� ���ؽ� ���̴��� �ȼ� ���̴��� �ڵ� �и� ������
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
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr; // �߰� ������ �����
	std::string EntryName;

	// ���ε� ����(�̸�, �ε���, Ÿ��)�� ����ϴ� �Լ�
	void ReflectAndBindShaderResources();

private:
	// delete Function
	UEngineShaderBase(const UEngineShaderBase& _Other) = delete;
	UEngineShaderBase(UEngineShaderBase&& _Other) noexcept = delete;
	UEngineShaderBase& operator=(const UEngineShaderBase& _Other) = delete;
	UEngineShaderBase& operator=(UEngineShaderBase&& _Other) noexcept = delete;
};

