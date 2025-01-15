#include "PreCompile.h"
#include "RoomManager.h"
#include "Room.h"

URoomManager::URoomManager()
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
	ARoom* Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + PNG, DirtmouthStr + BMP, { 13652, 3666 });
	ARoom* ForgottenCrossroads1 = CreateRoom(ForgottenCrossroadsStr1, ForgottenCrossroadsStr1 + PNG, ForgottenCrossroadsStr1 + BMP, { 5878, 2826 });


	//Rooms.push_back(ForgottenCrossroads1);

	Dirtmouth->InterLinkRoom(ForgottenCrossroads1);
	Dirtmouth->SetInitPos({ 0, 100 });
	ForgottenCrossroads1->SetInitPos({ 6050.0f, -3666.0f / 2.0f - 3500.0f / 2.0f });
	SetInitCurRoom(Dirtmouth);

	//SetInitCurRoom(ForgottenCrossroads1);
	//ForgottenCrossroads1->SetActorLocation({ 2000, 1000 });

	//Dirtmouth->CreateDoor({ 1500, -2600 }, Dirtmouth, {1100, -2600});
	// ForgottenCrossroads1->CreateDoor({ 0, 0 }, ForgottenCrossroads1, {100, -100});
}

ARoom* URoomManager::CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio/* = 1.0f*/)
{
	std::string RoomName = _RoomName.data();
	ARoom* NewRoom = GameMode->GetWorld()->SpawnActor<ARoom>().get();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);
	NewRoom->SetActorLocation({ _Size.X / 2.0f * _ScaleRatio, -_Size.Y / 2.0f * _ScaleRatio });

	NewRoom->CreateTexture(_BackgroundName, _ScaleRatio);
	LoadPixelCollisionTexture(NewRoom, &NewRoom->GetPixelCollisionImage(), _PixelCollisionName, _Size, _ScaleRatio);
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

	_Room->CreatePixelCollisionTexture(_FileName, _ScaleRatio);

}

