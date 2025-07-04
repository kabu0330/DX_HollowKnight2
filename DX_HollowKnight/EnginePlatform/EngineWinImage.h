#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>

class UEngineWinImage : public UObject
{
public:
	ENGINEAPI UEngineWinImage();
	ENGINEAPI ~UEngineWinImage();

	ENGINEAPI HDC GetDC()
	{
		return ImageDC;
	}
	
	ENGINEAPI void CreateWinImageHDC(HDC _DC)
	{
		ImageDC = _DC;
	}

	ENGINEAPI void CreateWinImage(UEngineWinImage* _TargetImage, FVector _Scale);

	ENGINEAPI void CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans);

	ENGINEAPI void CopyToTrans(UEngineWinImage* _TargetImage,
		const FTransform& _RenderTrans, 
		const FTransform& _LTImageTrans,
		UColor _Color = UColor(255, 0, 255, 0));

	ENGINEAPI void CopyToAlpha(UEngineWinImage* _TargetImage,
		const FTransform& _RenderTrans,
		const FTransform& _LTImageTrans,
		unsigned char _Alpha);


	ENGINEAPI void LoadWinImage(UEngineWinImage* _TargetImage, std::string_view _Path);

	ENGINEAPI FVector GetImageScale() const
	{
		return { Info.bmWidth, Info.bmHeight };
	}

	ENGINEAPI UColor GetColor(FVector _Point, UColor _DefaultColor = UColor{255, 255, 255, 255})
	{
		return GetColor(_Point.ConvertToPoint(), _DefaultColor);
	}

	ENGINEAPI UColor GetColor(FIntPoint _Point, UColor _DefaultColor);

protected:

private:
	HDC ImageDC = nullptr;
	HBITMAP hBitMap = nullptr;

	BITMAP Info;

private:
	// delete Function
	UEngineWinImage(const UEngineWinImage& _Other) = delete;
	UEngineWinImage(UEngineWinImage&& _Other) noexcept = delete;
	UEngineWinImage& operator=(const UEngineWinImage& _Other) = delete;
	UEngineWinImage& operator=(UEngineWinImage&& _Other) noexcept = delete;
};

