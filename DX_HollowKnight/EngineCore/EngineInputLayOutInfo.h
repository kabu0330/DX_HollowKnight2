#pragma once

// Ό³Έν :
class UEngineInputLayoutInfo
{
public:
	ENGINEAPI UEngineInputLayoutInfo();
	ENGINEAPI ~UEngineInputLayoutInfo();


	ENGINEAPI void AddInputLayout(
		LPCSTR _SemanticName,
		DXGI_FORMAT _Format,
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
		UINT _InstanceDataStepRate = 0,
		UINT _AlignedByteOffset = -1,
		UINT _InputSlot = 0,
		UINT _SemanticIndex = 0
	);

	ENGINEAPI UINT GetFormatSize(DXGI_FORMAT _Value);

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayOutData;

protected:

private:
	int Offset = 0;

private:
	// delete Function
	UEngineInputLayoutInfo(const UEngineInputLayoutInfo& _Other) = delete;
	UEngineInputLayoutInfo(UEngineInputLayoutInfo&& _Other) noexcept = delete;
	UEngineInputLayoutInfo& operator=(const UEngineInputLayoutInfo& _Other) = delete;
	UEngineInputLayoutInfo& operator=(UEngineInputLayoutInfo&& _Other) noexcept = delete;
};

