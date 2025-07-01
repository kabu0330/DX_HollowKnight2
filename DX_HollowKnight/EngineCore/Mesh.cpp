#include "PreCompile.h"
#include "Mesh.h"

UMesh::UMesh()
{
}

UMesh::~UMesh()
{
}

std::shared_ptr<UMesh> UMesh::CreateWithBuffers(std::string_view _Name, std::string_view _VertexBuffer, std::string_view _IndexBuffer)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 등록된 메시입니다. \n" + UpperName);
		return nullptr;
	}

	std::shared_ptr<UMesh> NewRes = std::make_shared<UMesh>();
	PushRes<UMesh>(NewRes, _Name, "");
	NewRes->VertexBuffer = UEngineVertexBuffer::Find<UEngineVertexBuffer>(_VertexBuffer);
	NewRes->IndexBuffer = UEngineIndexBuffer::Find<UEngineIndexBuffer>(_IndexBuffer);

	return NewRes;
}
