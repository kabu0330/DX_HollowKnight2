#include "PreCompile.h"
#include "RoomManager.h"
#include "Room.h"

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
	std::string ForgottenCrossroadsStr1 = "ForgottenCrossroads1";
	std::string ForgottenCrossroadsStr2 = "ForgottenCrossroads2";
	std::string ForgottenCrossroadsStr3 = "ForgottenCrossroads3";

	// 맵 세팅
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + PNG, DirtmouthStr + BMP, { 13652, 3666 });
	std::shared_ptr<ARoom> ForgottenCrossroads1 = CreateRoom(ForgottenCrossroadsStr1, ForgottenCrossroadsStr1 + PNG, ForgottenCrossroadsStr1 + BMP, { 5878, 2826 });

	Rooms.reserve(10);
	Rooms.push_back(Dirtmouth);
	//Rooms.push_back(ForgottenCrossroads1);

	Dirtmouth->SetRoomLocation({ 100, 100 });
	ForgottenCrossroads1->SetRoomLocation({ 6150, -3550 });
	SetInitCurRoom(Dirtmouth.get());
	//SetInitCurRoom(ForgottenCrossroads1.get());


	Dirtmouth->CreateDoor({ 9418, -3308 }, ForgottenCrossroads1.get(), { 9384, -5595 });
	// ForgottenCrossroads1->CreateDoor({ 0, 0 }, ForgottenCrossroads1, {100, -100});
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

