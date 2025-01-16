#include "PreCompile.h"
#include "RoomManager.h"
#include "Room.h"
#include "ContentsConst.h"
#include "Door.h"

URoomManager::URoomManager()
	: Rooms()
{
}

URoomManager::~URoomManager()
{
}

void URoomManager::CreateAndLinkRoom(AGameMode* _GameMode)
{
	GameMode = _GameMode;
	std::string PNG = "_back.png";
	std::string BMP = "_pixel.bmp";
	std::string DirtmouthStr = "Dirtmouth";
	std::string CrossroadsStr1 = "ForgottenCrossroads1";
	std::string CrossroadsStr2 = "ForgottenCrossroads2";
	std::string CrossroadsStr3 = "ForgottenCrossroads3";

	// 맵 세팅
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + PNG, DirtmouthStr + BMP, { 13652, 3666 });
	std::shared_ptr<ARoom> Crossroads1 = CreateRoom(CrossroadsStr1, CrossroadsStr1 + PNG, CrossroadsStr1 + BMP, { 5878, 2826 });
	std::shared_ptr<ARoom> Crossroads2 = CreateRoom(CrossroadsStr2, CrossroadsStr2 + PNG, CrossroadsStr2 + BMP, { 5752, 2470 });

	Rooms.reserve(10);
	Rooms.push_back(Dirtmouth);
	Rooms.push_back(Crossroads1);
	Rooms.push_back(Crossroads2);

	Dirtmouth->SetRoomLocation({ 100, 100 });
	Crossroads1->SetRoomLocation({ 6150, -3550 });
	Crossroads2->SetRoomLocation({ 6120 + 5952, -3580 });

	//SetInitCurRoom(ForgottenCrossroads1.get());

	//                      문 크기               문 위치                    도착 맵             도착 위치              입력 키 받을건지?
	ADoor* Door0 =   Dirtmouth->CreateDoor({ 300, 100 }, RoomPos::FromDirtmouth,   Crossroads1.get(), RoomPos::ToCrossroads1      );
	ADoor* Door1 = Crossroads1->CreateDoor({ 300, 200 }, RoomPos::FromCrossroads1, Dirtmouth.get()  , RoomPos::ToDirtmouth  , true);
	ADoor* Door2 = Crossroads1->CreateDoor({ 100, 300 }, RoomPos::Crossroads1Right, Crossroads2.get(), RoomPos::Crossroads2Left);
	ADoor* Door3 = Crossroads1->CreateDoor({ 100, 300 }, RoomPos::Crossroads2Left, Crossroads2.get(), RoomPos::Crossroads1Right);
	Door0->GetCollision()->SetDebugColor({ 1.0f, 0.0f, 1.0f, 1.0f }); 
	// ForgottenCrossroads1->CreateDoor({ 0, 0 }, ForgottenCrossroads1, {100, -100});

	SetInitCurRoom(Dirtmouth.get());
}

std::shared_ptr<ARoom> URoomManager::CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio/* = 1.0f*/)
{
	std::string RoomName = _RoomName.data();
	std::shared_ptr<ARoom> NewRoom = GameMode->GetWorld()->SpawnActor<ARoom>();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);
	float ZOrder = static_cast<float>(EZOrder::BACKGROUND);
	NewRoom->SetActorLocation({ _Size.X / 2.0f , -_Size.Y / 2.0f, ZOrder });

	NewRoom->CreateTexture(_BackgroundName, _ScaleRatio);
	LoadPixelCollisionTexture(NewRoom.get(), &NewRoom->GetPixelCollisionImage(), _PixelCollisionName, _Size, _ScaleRatio);
	return NewRoom;
}

void URoomManager::SetInitCurRoom(ARoom* _InitRoom)
{
	ARoom::SetCurRoom(_InitRoom);
}

void URoomManager::LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size, float _ScaleRatio)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append("MapData");
	UEngineFile ImageFiles = Dir.GetFile(_FileName);

	_BmpTexture->Load(nullptr, ImageFiles.GetPathToString()); // 픽셀 충돌 파일 설정

}

