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
	//ARoom* Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + PNG, DirtmouthStr + BMP, { 13652, 3666 });
	ARoom* ForgottenCrossroads1 = CreateRoom(ForgottenCrossroadsStr1, ForgottenCrossroadsStr1 + PNG, ForgottenCrossroadsStr1 + BMP, { 5878, 2826 });
	
	
	//Dirtmouth->InterLinkRoom(ForgottenCrossroads1, FVector{ 13652.0f * 0.445f ,-3666.0f });

	ForgottenCrossroads1->SetActorLocation({ 0.0f, 0.0f });
	//SetInitCurRoom(Dirtmouth);
	SetInitCurRoom(ForgottenCrossroads1);

	//Dirtmouth->CreateDoor({ 1500, -2600 }, Dirtmouth, {1100, -2600});
	ForgottenCrossroads1->CreateDoor({ 0, 0 }, ForgottenCrossroads1, {100, -100});
}

ARoom* URoomManager::CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio/* = 1.0f*/)
{
	std::string RoomName = _RoomName.data();
	std::shared_ptr<ARoom> NewRoom = GameMode->GetWorld()->SpawnActor<ARoom>();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);
	NewRoom->SetActorLocation({ _Size.X / 2.0f * _ScaleRatio, -_Size.Y / 2.0f * _ScaleRatio });

	NewRoom->CreateTexture(_BackgroundName, _ScaleRatio);
	LoadPixelCollisionTexture(NewRoom.get(), &NewRoom->GetPixelCollisionImage(), _PixelCollisionName, _Size, _ScaleRatio);
	return NewRoom.get();
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

	_BmpTexture->Load(nullptr, ImageFiles.GetPathToString());

	_Room->CreatePixelCollisionTexture(_FileName, _ScaleRatio);

}

