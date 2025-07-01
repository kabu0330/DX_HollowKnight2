#pragma once
#include "EngineIndexBuffer.h"
#include "EngineVertexBuffer.h"

// Ό³Έν :
class UMesh : public UEngineResourceBase
{
public:
	UMesh();
	~UMesh();

	static std::shared_ptr<UMesh> Create(std::string_view _Name)
	{
		return CreateWithBuffers(_Name, _Name, _Name);
	}

	static std::shared_ptr<UMesh> CreateWithBuffers(std::string_view _Name, std::string_view _VertexBuffer, std::string_view _IndexBuffer);

	std::shared_ptr<UEngineVertexBuffer> GetVertexBuffer()
	{
		return VertexBuffer;
	}

	std::shared_ptr<UEngineIndexBuffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}

protected:

private:
	std::shared_ptr<UEngineVertexBuffer> VertexBuffer;
	std::shared_ptr<UEngineIndexBuffer> IndexBuffer;

private:
	// delete Function
	UMesh(const UMesh& _Other) = delete;
	UMesh(UMesh&& _Other) noexcept = delete;
	UMesh& operator=(const UMesh& _Other) = delete;
	UMesh& operator=(UMesh&& _Other) noexcept = delete;

};

