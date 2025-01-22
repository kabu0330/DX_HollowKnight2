#include "PreCompile.h"
#include "RoomManager.h"
#include "Room.h"
#include "Door.h"
#include "Platform.h"

URoomManager::URoomManager()
	: Rooms()
{
}

URoomManager::~URoomManager()
{
}

void URoomManager::CreateRoom(AGameMode* _GameMode)
{
	GameMode = _GameMode;
	std::string PNG = "_back.png";
	std::string BMP = "_pixel.bmp";
	std::string DirtmouthStr = "Dirtmouth";
	std::string CrossroadsStr1 = "ForgottenCrossroads1";
	std::string CrossroadsStr2 = "ForgottenCrossroads2";
	std::string CrossroadsStr3 = "ForgottenCrossroads3";

	// 맵 세팅
	// 1. 생성
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + PNG, DirtmouthStr + BMP, { 13652, 3666 });
	std::shared_ptr<ARoom> Crossroads1 = CreateRoom(CrossroadsStr1, CrossroadsStr1 + PNG, CrossroadsStr1 + BMP, { 5878, 2826 });
	std::shared_ptr<ARoom> Crossroads2 = CreateRoom(CrossroadsStr2, CrossroadsStr2 + PNG, CrossroadsStr2 + BMP, { 5752, 2470 });
	std::shared_ptr<ARoom> Crossroads3 = CreateRoom(CrossroadsStr3, CrossroadsStr3 + PNG, CrossroadsStr3 + BMP, { 3150, 6746 });

	// 2. push_back
	Rooms.reserve(10);
	Rooms.push_back(Dirtmouth);
	Rooms.push_back(Crossroads1);
	Rooms.push_back(Crossroads2);
	Rooms.push_back(Crossroads3);

	// 3. 픽셀충돌 위치지정
	Dirtmouth->SetRoomLocation({ 0, 0 });
	Crossroads1->SetRoomLocation({ 6150, -3550 });
	Crossroads2->SetRoomLocation({ 12572, -3580 });
	Crossroads3->SetRoomLocation({ 2400, -4450 });


	// 4. 포탈 or 문 생성
	//                      문 크기               문 위치                    도착 맵             도착 위치              입력 키 받을건지?
	  Dirtmouth->CreateDoor({ 300, 100 }, RoomPos::FromDirtmouth,   Crossroads1.get(), RoomPos::ToCrossroads1      );

	// 교차로 1
	Crossroads1->CreateDoor({ 300, 200 }, RoomPos::FromCrossroads1 , Dirtmouth.get()  , RoomPos::ToDirtmouth    , true);
	Crossroads1->CreateDoor({ 50, 300 }, RoomPos::Crossroads1Right, Crossroads2.get(), RoomPos::Crossroads2Left      );
	Crossroads1->CreateDoor({ 50, 300 }, RoomPos::Crossroads1Left , Crossroads3.get(), RoomPos::Crossroads3Right     );

	// 교차로 2 좌상단
	Crossroads2->CreateDoor({ 50, 300 }, RoomPos::Crossroads2Left, Crossroads1.get(), RoomPos::Crossroads1Right);

	// 교차로 3 우측
	Crossroads2->CreateDoor({ 50, 300 }, RoomPos::Crossroads3Right, Crossroads1.get(), RoomPos::Crossroads1Left);
	// ForgottenCrossroads1->CreateDoor({ 0, 0 }, ForgottenCrossroads1, {100, -100});

	// 5. 시작 위치 중력 설정 : 문과 충돌해야만 픽셀충돌 맵이 지정되기 때문에 여기서 강제로 설정 필요한 경우 사용
	//SetInitCurRoom(ForgottenCrossroads1.get());
	//SetInitCurRoom(Dirtmouth.get());
	SetInitCurRoom(Crossroads1.get());

	// 6. 몬스터 생성
	//Dirtmouth->CreateMonster<AMonster>({ 1300, -2700 });
	Crossroads1->CreateMonster<AMonster>({ 11100, -5800 });

	//Crossroads1->CreateObject<APlatform>({10250, -5680}, {150, 40});
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

