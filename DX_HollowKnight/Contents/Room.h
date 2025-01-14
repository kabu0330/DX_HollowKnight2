#pragma once
#include <EngineCore/Actor.h>

// 설명 :
class ARoom : public AActor
{
public:
	// constrcuter destructer
	ARoom();
	~ARoom();

	// delete Function
	ARoom(const ARoom& _Other) = delete;
	ARoom(ARoom&& _Other) noexcept = delete;
	ARoom& operator=(const ARoom& _Other) = delete;
	ARoom& operator=(ARoom&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UEngineWinImage& GetPixelCollisionImage()
	{
		return PixelCollisionImage;
	}
	static ARoom* GetCurRoom()
	{
		return CurRoom;
	}
	static void SetCurRoom(ARoom* _Room)
	{
		CurRoom = _Room;
	}
	
	bool IsLinking(ARoom* _Room);
	bool InterLinkRoom(ARoom* _Room, FVector _WorldPos);
	ARoom* LinkRoom(ARoom* _Room);
	//std::shared_ptr<ARoom> LinkRoom(ARoom* _Room);


	void CreateTexture(std::string_view _FileName, float _ScaleRatio);
	void CreatePixelCollisionTexture(std::string_view _FileName, float _ScaleRatio);

	FVector GetSize() const
	{
		return Size;
	}
	void SetSize(const FVector& _Size)
	{
		Size = _Size;
	}

	// 중력과 벽
	void CheckPixelCollisionWithGravity(AActor* _Actor, class UContentsRenderer* _Renderer);
	void CheckPixelCollisionWithWall(AActor* _Actor, class UContentsRenderer* _Renderer, float _Speed, bool _Left);

protected:
	void Gravity(AActor* _Actor, float _DeltaTime);
	void BlockByWall(AActor* _Actor, float _Speed, float _DeltaTime);

private:
	inline static bool bActiveGravity = false;
	float GravityValue = 0.0f;
	FVector GravityForce = FVector::ZERO;

	inline static ARoom* CurRoom = nullptr;
	UEngineWinImage PixelCollisionImage;
	FVector Size = FVector::ZERO;
	FVector Pos = FVector::ZERO;

	std::shared_ptr<class UContentsRenderer> PixelCollisionTexture;
	std::shared_ptr<class UContentsRenderer> BackgroundRenderer;

	std::vector<ARoom*> Rooms;
};

