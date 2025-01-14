#pragma once
#include <EngineBase/EngineSerializer.h>
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"

//class MapObjectData : public ISerializeObject
//{
//public:
//	//USpriteRenderer* SpriteRenderer;
//	//bool bIsCollision = false; // 충돌여부
//	//bool bIsInteractable = false; // 통과 여부
//
//	//FVector Scale;
//	//FVector Position;
//
//	//ELayer Layer;
//
//	std::shared_ptr<USpriteRenderer> SpriteRenderer;
//	bool IsMove = true;
//	int TileType = -1;
//
//	FVector Scale;
//	FVector Pivot;
//	int SpriteIndex;
//
//	void Serialize(UEngineSerializer& _Ser) override
//	{
//		std::string SpriteName;
//		if (nullptr != SpriteRenderer)
//		{
//			SpriteName = SpriteRenderer->GetCurSpriteName();
//		}
//		_Ser << SpriteName;
//		_Ser << IsMove;
//		_Ser << TileType;
//		_Ser << Scale;
//		_Ser << Pivot;
//		_Ser << SpriteIndex;
//	}
//
//	void DeSerialize(UEngineSerializer& _Ser) override
//	{
//		std::string SpriteName;
//		_Ser >> SpriteName;
//		_Ser >> IsMove;
//		_Ser >> TileType;
//		_Ser >> Scale;
//		_Ser >> Pivot;
//		_Ser >> SpriteIndex;
//	}
//
//};
//
//// 설명 :
//class AMapObject : public AActor, public ISerializeObject
//{
//public:
//	// constrcuter destructer
//	AMapObject();
//	~AMapObject();
//
//	// delete Function
//	AMapObject(const AMapObject& _Other) = delete;
//	AMapObject(AMapObject&& _Other) noexcept = delete;
//	AMapObject& operator=(const AMapObject& _Other) = delete;
//	AMapObject& operator=(AMapObject&& _Other) noexcept = delete;
//
//	void Create(std::string_view _Sprite, FIntPoint _Count, FVector _TileSize);
//
//	void SetTileLocation(FVector _Location, int _SpriteIndex);
//
//	void SetTileIndex(FIntPoint _Index, int _SpriteIndex);
//	void SetTileIndex(FIntPoint _Index, FVector _Pivot, FVector _SpriteScale, int _SpriteIndex);
//
//	std::shared_ptr<MapObjectData> GetTileRef(FIntPoint _Index);
//	std::shared_ptr<MapObjectData> GetTileRef(FVector _Location);
//
//	FVector IndexToTileLocation(FIntPoint _Index);
//
//	FIntPoint LocationToIndex(FVector _Location);
//
//	bool IsIndexOver(FIntPoint _Index);
//
//	void Serialize(UEngineSerializer& _Ser);
//	void DeSerialize(UEngineSerializer& _Ser);
//
//
//	void BeginPlay();
//	void Tick(float _DeltaTime);
//
//protected:
//
//private:
//	FIntPoint TileCount;
//	std::string SpriteName;
//	FVector TileSize;
//	std::vector<std::vector<MapObjectData>> AllTiles;
//};

