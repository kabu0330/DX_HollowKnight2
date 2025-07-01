#include "PreCompile.h"
#include "EngineVertex.h"

UEngineInputLayoutInfo FEngineVertex::InputLayoutInfo;

UEngineInputLayoutInfo& FEngineVertex::GetInputLayoutInfo()
{
	return InputLayoutInfo;
}

class EngineVertexInit
{
public:
	ENGINEAPI EngineVertexInit()
	{
		FEngineVertex::InputLayoutInfo.AddInputLayout("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
		FEngineVertex::InputLayoutInfo.AddInputLayout("TEXCOORD", DXGI_FORMAT_R32G32B32A32_FLOAT);
		FEngineVertex::InputLayoutInfo.AddInputLayout("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	}
};

EngineVertexInit InitObject;