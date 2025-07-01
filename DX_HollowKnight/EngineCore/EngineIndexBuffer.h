#pragma once
#include "EngineResourceBase.h"
#include "EngineDeviceBufferBase.h"

// Ό³Έν :
class UEngineIndexBuffer : public UEngineResourceBase, public UEngineDeviceBufferBase
{
public:
	UEngineIndexBuffer();
	~UEngineIndexBuffer();

	template<typename IndexType>
	static std::shared_ptr<UEngineIndexBuffer> CreateIndexBuffer(std::string_view _Name, const std::vector<IndexType>& _Data)
	{
		return CreateIndexBuffer(_Name, reinterpret_cast<const void*>(&_Data[0]), sizeof(IndexType), _Data.size());
	}

	static std::shared_ptr<UEngineIndexBuffer> CreateIndexBuffer(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount);

	void IASetIndexBuffer();

	ENGINEAPI UINT GetIndexCount()
	{
		return IndexCount;
	}

protected:
	void CreateIndexBuffer(const void* _InitData, size_t _VertexSize, size_t _VertexCount);

private:
	UINT IndexSize = 0;
	UINT IndexCount = 0;
	DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;

private:
	// delete Function
	UEngineIndexBuffer(const UEngineIndexBuffer& _Other) = delete;
	UEngineIndexBuffer(UEngineIndexBuffer&& _Other) noexcept = delete;
	UEngineIndexBuffer& operator=(const UEngineIndexBuffer& _Other) = delete;
	UEngineIndexBuffer& operator=(UEngineIndexBuffer&& _Other) noexcept = delete;
};

