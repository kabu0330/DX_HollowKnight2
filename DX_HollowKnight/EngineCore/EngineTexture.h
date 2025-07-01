#pragma once
#include "EngineResourceBase.h"
#include "EngineEnums.h"

#include "ThirdParty/DirectxTex/Inc/DirectXTex.h"

// 설명 : 텍스처의 
// 1. 상수버퍼 세팅, 
// 2.SRV 생성, 
// 3. DSV 생성
// 4. 렌더 타겟 뷰 생성
class UEngineTexture : public UEngineResourceBase
{
	friend class UEngineRenderTarget;

public:
	ENGINEAPI UEngineTexture();
	ENGINEAPI ~UEngineTexture();

	static std::shared_ptr<UEngineTexture> LoadTextureThreadSafe(std::string_view _Path)
	{
		UEnginePath EnginePath = UEnginePath(_Path);
		std::string FileName = EnginePath.GetFileNameToString();
		return LoadTextureThreadSafe(FileName, _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineTexture> LoadTextureThreadSafe(std::string_view _Name, std::string_view _Path);


	static std::shared_ptr<UEngineTexture> LoadTexture(std::string_view _Path)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileNameToString();

		return LoadTexture(FileName, _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineTexture> LoadTexture(std::string_view _Name, std::string_view _Path);

	ID3D11DepthStencilView* GetDSV()
	{
		return DSV.Get();
	}

	ID3D11RenderTargetView* GetRTV()
	{
		return RTV.Get();
	}

	ID3D11ShaderResourceView* GetSRV()
	{
		return SRV.Get();
	}

	FVector GetTextureSize()
	{
		return Size;
	}

	void BindToShaderResources(EShaderType _Type, UINT _BindIndex);
	void UnbindFromShaderResources(EShaderType _Type, UINT _BindIndex);

	ENGINEAPI void CreateTextureWithView(const D3D11_TEXTURE2D_DESC& _Value);
	ENGINEAPI void CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D);

protected:

private:
	ENGINEAPI void CreateRenderTargetView();
	ENGINEAPI void CreateShaderResourceView();
	ENGINEAPI void CreateDepthStencilView();

	ENGINEAPI void LoadFromFile();
	ENGINEAPI void CreateTexShaderResourceView();

	FVector Size = FVector::ZERO;
	DirectX::TexMetadata Metadata = DirectX::TexMetadata();
	DirectX::ScratchImage ImageData = DirectX::ScratchImage();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // 로드한 텍스처
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DSV = nullptr; // 텍스처를 쉐이더 세팅할수 있는권한
	D3D11_TEXTURE2D_DESC Desc;

private:
	// delete Function
	UEngineTexture(const UEngineTexture& _Other) = delete;
	UEngineTexture(UEngineTexture&& _Other) noexcept = delete;
	UEngineTexture& operator=(const UEngineTexture& _Other) = delete;
	UEngineTexture& operator=(UEngineTexture&& _Other) noexcept = delete;
};
