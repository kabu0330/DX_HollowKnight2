#include "PreCompile.h"
#include "EngineIndexBuffer.h"

UEngineIndexBuffer::UEngineIndexBuffer()
{
}

UEngineIndexBuffer::~UEngineIndexBuffer()
{
}

std::shared_ptr<UEngineIndexBuffer> UEngineIndexBuffer::CreateIndexBuffer(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 등록된 인덱스 버퍼입니다. \n" + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineIndexBuffer> NewRes = std::make_shared<UEngineIndexBuffer>();
	PushResource<UEngineIndexBuffer>(NewRes, _Name, "");
	NewRes->CreateIndexBuffer(_InitData, _VertexSize, _VertexCount);

	return NewRes;
}

void UEngineIndexBuffer::CreateIndexBuffer(const void* _InitData, size_t _Size, size_t _Count)
{
	IndexSize = static_cast<UINT>(_Size);
	IndexCount = static_cast<UINT>(_Count);

	if (4 == IndexSize) // 42억
	{
		Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	}
	else if (2 == IndexSize) // 65536
	{
		Format = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
	}
	else
	{
		MSGASSERT("해당 인덱스 포맷은 지원하지 않습니다.");
	}

	BufferInfo.ByteWidth = static_cast<UINT>(_Size * _Count);
	BufferInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data; // 초기값 넣어주는 용도의 구조체
	Data.pSysMem = _InitData;

	if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		MSGASSERT("인덱스 버퍼 생성에 실패했습니다.");
		return;
	}
}

void UEngineIndexBuffer::IASetIndexBuffer()
{
	int Offset = 0;
	UEngineCore::GetDevice().GetContext()->IASetIndexBuffer(Buffer.Get(), Format, Offset);
}