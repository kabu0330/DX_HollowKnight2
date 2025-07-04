#pragma once
#include <wrl.h>
#include <d3d11_4.h> 
#include <d3dcompiler.h> 
#include <EnginePlatform/EngineWindow.h>
#include <memory>


#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI") 

// 설명 : Device와 Context, 스왑체인과 백버퍼, RTV, DSV 생성, 백버퍼 초기화 및 출력
class UEngineGraphicDevice
{
public:
	ENGINEAPI UEngineGraphicDevice();
	ENGINEAPI ~UEngineGraphicDevice();

	void SetupRenderingPipeline();
	void CreateBackBuffer(const UEngineWindow& _Window);

	IDXGIAdapter* GetHighPerformanceAdapter();

	ENGINEAPI void Release();

	void StartRender();
	void EndRender();

	ENGINEAPI ID3D11Device* GetDevice()
	{
		return Device.Get();
	}

	ENGINEAPI ID3D11DeviceContext* GetContext()
	{
		return Context.Get();
	}

	ENGINEAPI std::shared_ptr<class UEngineRenderTarget> GetBackBufferTarget()
	{
		return BackBufferTarget;
	}

protected:

private:
	Microsoft::WRL::ComPtr<IDXGIAdapter> MainAdapter = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;

	std::shared_ptr<class UEngineRenderTarget> BackBufferTarget;

	
	ENGINEAPI void InitDefaultResources();

	ENGINEAPI void InitMesh();
	ENGINEAPI void InitBlend();
	ENGINEAPI void InitShader();
	ENGINEAPI void InitMaterial();
	ENGINEAPI void InitRasterizerState();
	ENGINEAPI void InitTexture();
	ENGINEAPI void InitDepthStencil();

private:
	// delete Function
	UEngineGraphicDevice(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& _Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& _Other) noexcept = delete;
};

