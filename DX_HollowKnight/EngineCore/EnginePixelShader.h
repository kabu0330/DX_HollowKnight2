#pragma once
#include "EngineResourceBase.h"
#include <Windows.h>
#include "EngineShaderBase.h"

// Ό³Έν :
class UEnginePixelShader : public UEngineResourceBase, public UEngineShaderBase
{
public:
	UEnginePixelShader();
	~UEnginePixelShader();

	static std::shared_ptr<UEnginePixelShader> LoadPixelShader(std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileNameToString();

		return LoadPixelShader(FileName, _Path, _EntryPoint, _VersionHigh, _VersionLow);
	}

	ENGINEAPI static std::shared_ptr<UEnginePixelShader> LoadPixelShader(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh = 5, UINT _VersionLow = 0);

	void PSSetShader();

protected:

private:
	ENGINEAPI void CreatePixelShader();

	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader = nullptr;

private:
	// delete Function
	UEnginePixelShader(const UEnginePixelShader& _Other) = delete;
	UEnginePixelShader(UEnginePixelShader&& _Other) noexcept = delete;
	UEnginePixelShader& operator=(const UEnginePixelShader& _Other) = delete;
	UEnginePixelShader& operator=(UEnginePixelShader&& _Other) noexcept = delete;
};

