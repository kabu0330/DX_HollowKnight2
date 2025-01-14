#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineCore.h>

#pragma comment(lib, "EngineBase.lib")
#pragma comment(lib, "EnginePlatform.lib")
#pragma comment(lib, "EngineCore.lib")

#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")

int APIENTRY wWinMain(_In_ HINSTANCE _hInstance,
	_In_opt_ HINSTANCE _hPrevInstance,
	_In_ LPWSTR    _lpCmdLine,
	_In_ int       _nCmdShow)
{
	UEngineCore::EngineStart(_hInstance, "Contents.dll");

	IDXGIDebug* dxgiDebug = nullptr;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug)))) {
		dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
		dxgiDebug->Release();
	}
	return 0;
}

