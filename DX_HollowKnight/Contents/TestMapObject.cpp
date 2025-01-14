#include "PreCompile.h"
#include "TestMapObject.h"

//void AMapObject::Create(std::string_view _Sprite, FIntPoint _Count, FVector _TileSize)
//{
//	SpriteName = _Sprite;
//	TileSize = _TileSize;
//	TileCount = _Count;
//
//	AllTiles.resize(_Count.Y);
//
//	for (size_t y = 0; y < AllTiles.size(); y++)
//	{
//		AllTiles[y].resize(_Count.X);
//	}
//}
//
//void AMapObject::SetTileLocation(FVector _Location, int _SpriteIndex)
//{
//	//FVector Pos = RootComponent->GetTransformRef().RelativeLocation;
//	FVector Pos = RootComponent->GetTransformRef().WorldLocation;
//	FVector TilePos = _Location - Pos;
//
//	FIntPoint Point = LocationToIndex(TilePos);
//
//	if (true == IsIndexOver(Point))
//	{
//		return;
//	}
//
//	SetTileIndex(Point, _SpriteIndex);
//}
//
//void AMapObject::SetTileIndex(FIntPoint _Index, int _SpriteIndex)
//{
//	SetTileIndex(_Index, { 0,0 }, TileSize, _SpriteIndex);
//}
//
//void AMapObject::SetTileIndex(FIntPoint _Index, FVector _Pivot, FVector _SpriteScale, int _SpriteIndex)
//{
//	if (true == IsIndexOver(_Index))
//	{
//		return;
//	}
//
//	if (nullptr == AllTiles[_Index.Y][_Index.X].SpriteRenderer)
//	{
//		std::shared_ptr<USpriteRenderer> NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
//		// AllTiles[TileCount.Y][TileCount.X].SpriteRenderer;
//		NewSpriteRenderer->SetRelativeScale3D(TileSize);
//		AllTiles[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
//	}
//
//	std::shared_ptr<USpriteRenderer> FindSprite = AllTiles[_Index.Y][_Index.X].SpriteRenderer;
//	FindSprite->SetSprite(SpriteName, _SpriteIndex);
//
//	FVector TileLocation = IndexToTileLocation(_Index);
//	FindSprite->SetRelativeScale3D(_SpriteScale);
//	FindSprite->SetOrder(_Index.Y);
//
//	AllTiles[_Index.Y][_Index.X].SpriteRenderer->SetRelativeLocation(TileLocation + TileSize.Half() + _Pivot);
//	AllTiles[_Index.Y][_Index.X].Pivot = _Pivot;
//	AllTiles[_Index.Y][_Index.X].Scale = _SpriteScale;
//	AllTiles[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
//}
//
//std::shared_ptr<MapObjectData> AMapObject::GetTileRef(FIntPoint _Index)
//{
//	if (true == IsIndexOver(_Index))
//	{
//		return nullptr;
//	}
//
//	return static_cast<std::shared_ptr<MapObjectData>>(&AllTiles[_Index.Y][_Index.X]);
//}
//
//std::shared_ptr<MapObjectData> AMapObject::GetTileRef(FVector _Location)
//{
//	FIntPoint Point = LocationToIndex(_Location);
//
//	return GetTileRef(Point);
//}
//
//FVector AMapObject::IndexToTileLocation(FIntPoint _Index)
//{
//	return FVector(_Index.X * TileSize.X, _Index.Y * TileSize.Y);
//}
//
//FIntPoint AMapObject::LocationToIndex(FVector _Location)
//{
//	FVector Location = _Location / TileSize;
//
//	return FIntPoint(Location.iX(), Location.iY());
//}
//
//bool AMapObject::IsIndexOver(FIntPoint _Index)
//{
//	if (0 > _Index.X)
//	{
//		return true;
//	}
//
//	if (0 > _Index.Y)
//	{
//		return true;
//	}
//
//	if (TileCount.X - 1 < _Index.X)
//	{
//		return true;
//	}
//
//	if (TileCount.Y - 1 < _Index.Y)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//void AMapObject::Serialize(UEngineSerializer& _Ser)
//{
//	_Ser << TileCount;
//	_Ser << TileSize;
//	_Ser << SpriteName;
//	_Ser << AllTiles;
//}
//
//void AMapObject::DeSerialize(UEngineSerializer& _Ser)
//{
//	_Ser >> TileCount;
//	_Ser >> TileSize;
//	_Ser >> SpriteName;
//	std::vector<std::vector<MapObjectData>> LoadTiles;
//	_Ser >> LoadTiles;
//
//	Create(SpriteName, TileCount, TileSize);
//
//	for (int y = 0; y < LoadTiles.size(); y++)
//	{
//		for (int x = 0; x < LoadTiles[y].size(); x++)
//		{
//			SetTileIndex({ x, y }, LoadTiles[y][x].Pivot, LoadTiles[y][x].Scale, LoadTiles[y][x].SpriteIndex);
//		}
//	}
//}
//
//AMapObject::AMapObject()
//{
//	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
//	RootComponent = Default;
//}
//
//AMapObject::~AMapObject()
//{
//}
//
//void AMapObject::BeginPlay()
//{
//}
//
//void AMapObject::Tick(float _DeltaTime)
//{
//}