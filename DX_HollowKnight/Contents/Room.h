#pragma once
#include <EngineCore/Actor.h>
#include "Monster.h"

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
	
	//bool IsLinking(ARoom* _Room);
	//bool InterLinkRoom(ARoom* _Room, FVector _OffsetPos);
	//ARoom* LinkRoom(ARoom* _Room);

	// 초기 세팅
	void CreateTexture(std::string_view _FileName, float _ScaleRatio);
	void SetRoomLocation(FVector _Pos);

	class ADoor* CreateDoor(FVector _DoorScale, FVector _InitPos, ARoom* _TargetRoom, FVector _TargetPos, bool _IsDoor = false);

	// 중력과 벽
	void CheckPixelCollisionWithGravity(AActor* _Actor, class UContentsRenderer* _Renderer);
	void CheckPixelCollisionWithWall(AActor* _Actor, class UContentsRenderer* _Renderer, float _Speed, bool _Left);
	void CheckPixelCollisionWithCeil(AActor* _Actor, class UContentsRenderer* _Renderer, float _Speed, bool _Left);
	bool IsOnGround(FVector _Pos);
	bool IsOnGround(AActor* _Actor, class UContentsRenderer* _Renderer, FVector _Pos);
	FVector GetPixelCollisionPoint(AActor* _Actor, class UContentsRenderer* _Renderer, FVector _Offset);

	FVector GetSize() const
	{
		return Size;
	}
	void SetSize(const FVector& _Size)
	{
		Size = _Size;
	}
	void SetLeftTopPos(const FVector& _LeftTopPos)
	{
		LeftTopPos = _LeftTopPos;
	}

	static void SwitchDebugActiveGravity()
	{
		bActiveGravity = !bActiveGravity;
	}
	static bool GetActiveGravity()
	{
		return bActiveGravity;
	}

	FVector GetGravityForce() const
	{
		return GravityForce;
	}

	template<typename MonsterType>
	AMonster* CreateMonster(FVector _Pos, bool _CanWorldPos = true)
	{
		AMonster* NewMonster = GetWorld()->SpawnActor<MonsterType>().get();
		if (false == _CanWorldPos)
		{
			NewMonster->SetActorLocation(this->GetActorLocation() + _Pos);
		}
		else
		{
			NewMonster->SetActorLocation(_Pos);
		}

		NewMonster->SetParentRoom(this);
		Monsters.push_back(NewMonster);
		return NewMonster;
	}


protected:
	void Gravity(AActor* _Actor, float _DeltaTime);

private:
	inline static bool bActiveGravity = false;
	float GravityValue = 0.0f;
	FVector GravityForce = FVector::ZERO;

	inline static ARoom* CurRoom = nullptr;
	UEngineWinImage PixelCollisionImage;
	FVector Size = FVector::ZERO;
	FVector LeftTopPos = FVector::ZERO;

	std::shared_ptr<class UContentsRenderer> PixelCollisionTexture;
	std::shared_ptr<class UContentsRenderer> BackgroundRenderer;

	class ADoor* Door = nullptr;

	std::list<AMonster*> Monsters;
};

