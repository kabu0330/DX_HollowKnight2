#pragma once
#include "EngineResourceBase.h"
#include "EngineDeviceBufferBase.h"
#include "EngineEnums.h"

// ���� :
class UEngineConstantBuffer : public UEngineResourceBase, public UEngineDeviceBufferBase
{
public:
	UEngineConstantBuffer();
	~UEngineConstantBuffer();

	// ������۸� ����ų� ã�ų�
	static std::shared_ptr<UEngineConstantBuffer> CreateOrFind(UINT _Byte, const std::string_view& _Name);

	void UpdateConstantBufferData(void* _Data, UINT _Size);
	void BindToShaderSlot(EShaderType _Type, UINT _BindIndex);

	static void Release();

protected:

private:
	//           ����Ʈ           ����� ���� �̸�                     �������
	// � �̸����� ������۸� �� ����Ʈ �����Ұų�?
	static std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> AllConstantBuffers;

	void CreateConstantBuffer(UINT _Byte);

private:
	// delete Function
	UEngineConstantBuffer(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer(UEngineConstantBuffer&& _Other) noexcept = delete;
	UEngineConstantBuffer& operator=(const UEngineConstantBuffer& _Other) = delete;
	UEngineConstantBuffer& operator=(UEngineConstantBuffer&& _Other) noexcept = delete;
};

