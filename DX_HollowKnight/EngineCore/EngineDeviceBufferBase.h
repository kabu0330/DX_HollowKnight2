#pragma once

// ���� : ���ۿ� ���� DESC ����ü�� ����� ���� Ŭ����
//        ��� ���۸� ����� ���ؼ��� �ݵ�� ��ӹ޾ƾ� �Ѵ�.
class UEngineDeviceBufferBase
{
public:
	UEngineDeviceBufferBase();
	~UEngineDeviceBufferBase();

	const D3D11_BUFFER_DESC& GetBufferInfo()
	{
		return BufferInfo;
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer = nullptr;
	D3D11_BUFFER_DESC BufferInfo = { 0 };

private:


private:
	// delete Function
	UEngineDeviceBufferBase(const UEngineDeviceBufferBase& _Other) = delete;
	UEngineDeviceBufferBase(UEngineDeviceBufferBase&& _Other) noexcept = delete;
	UEngineDeviceBufferBase& operator=(const UEngineDeviceBufferBase& _Other) = delete;
	UEngineDeviceBufferBase& operator=(UEngineDeviceBufferBase&& _Other) noexcept = delete;
};

