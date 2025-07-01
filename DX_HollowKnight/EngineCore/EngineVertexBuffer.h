#pragma once
#include "EngineResourceBase.h"
#include "EngineDeviceBufferBase.h"
#include "EngineVertex.h"

// Ό³Έν :
class UEngineVertexBuffer : public UEngineResourceBase, public UEngineDeviceBufferBase
{
public:
	UEngineVertexBuffer();
	~UEngineVertexBuffer();

	template<typename VertexType>
	static std::shared_ptr<UEngineVertexBuffer> CreateVertexBuffer(std::string_view _Name, const std::vector<VertexType>& _VertexData)
	{
		return CreateVertexBuffer(_Name, reinterpret_cast<const void*>(&_VertexData[0]), sizeof(VertexType), _VertexData.size(), &VertexType::InputLayoutInfo);
	}

	static std::shared_ptr<UEngineVertexBuffer> CreateVertexBuffer(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount, UEngineInputLayoutInfo* _InfoPtr = nullptr);

	void IASetVertexBuffers();

	ENGINEAPI UEngineInputLayoutInfo* GetInputLayoutInfo()
	{
		return InputLayoutInfo;
	}

protected:
	void CreateVertexBuffer(const void* _InitData, size_t _VertexSize, size_t _VertexCount);

private:
	UINT VertexSize = 0;
	UINT VertexCount = 0;
	UEngineInputLayoutInfo* InputLayoutInfo;
	
private:
	// delete Function
	UEngineVertexBuffer(const UEngineVertexBuffer& _Other) = delete;
	UEngineVertexBuffer(UEngineVertexBuffer&& _Other) noexcept = delete;
	UEngineVertexBuffer& operator=(const UEngineVertexBuffer& _Other) = delete;
	UEngineVertexBuffer& operator=(UEngineVertexBuffer&& _Other) noexcept = delete;
};

