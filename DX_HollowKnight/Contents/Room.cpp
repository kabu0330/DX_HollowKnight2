#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"
#include "Monster.h"
#include "Door.h"

ARoom::ARoom()
{
	// 항상 DefaultSceneComponent는 생성자에서만. BeginPlay에서는 만들 수 없다.
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	//PixelCollisionTexture = CreateDefaultSubObject<UContentsRenderer>();
	//PixelCollisionTexture->SetupAttachment(RootComponent);
	//PixelCollisionTexture->SetAutoScaleRatio(1.0f);

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);

	//bActiveGravity = true;
}

ARoom::~ARoom()
{
}

void ARoom::BeginPlay()
{
	AActor::BeginPlay();

 }

void ARoom::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

//bool ARoom::IsLinking(ARoom* _Room)
//{
//	for (ARoom* Room : Rooms)
//	{
//		if (Room == _Room)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//bool ARoom::InterLinkRoom(ARoom* _Room, FVector _OffsetPos)
//{
//	this->LinkRoom(_Room);
//	_Room->LinkRoom(this);
//
//	return true;
//}
//
//ARoom* ARoom::LinkRoom(ARoom* _Room)
//{
//	if (_Room == this)
//	{
//		MSGASSERT("자기 자신을 연결할 수 없습니다.");
//		return nullptr;
//	}
//	if (true == IsLinking(_Room))
//	{
//		MSGASSERT("이미 연결된 맵입니다.");
//		return nullptr;
//	}
//	
//	Rooms.push_back(_Room);
//	return Rooms[Rooms.size() - 1];
//}

void ARoom::CreateTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);

	BackgroundRenderer->SetTexture(_FileName, true, _ScaleRatio);
}

void ARoom::SetRoomLocation(FVector _Pos)
{
	float ZOrder = static_cast<float>(EZOrder::BACKGROUND);
	SetActorLocation({ GetActorLocation().X + _Pos.X, GetActorLocation().Y + _Pos.Y, ZOrder });
	LeftTopPos = _Pos;
}

// 좌상단을 0, 0 기준으로 변경
ADoor* ARoom::CreateDoor(FVector _DoorScale, FVector _InitPos, ARoom* _TargetRoom, FVector _TargetPos, bool _IsDoor)
{
	Door = GetWorld()->SpawnActor<ADoor>().get();

	FVector RoomPos = { GetActorLocation().X - (Size.X / 2.0f), GetActorLocation().Y + (Size.Y / 2.0f) };
	//Door->SetActorLocation(RoomPos + _InitPos);
	Door->SetActorLocation( _InitPos);
	//FVector InitPos = RoomPos + _InitPos;

	Door->SetScale(_DoorScale);
	Door->SetWarpPosition(_TargetRoom, _TargetPos, _IsDoor);
	return Door;
}

// 중력
void ARoom::CheckPixelCollisionWithGravity(AActor* _Actor, class UContentsRenderer* _Renderer)
{
	float DeltaTime = UEngineCore::GetDeltaTime();

	//IsOnGround(_Actor, _Renderer);
	FVector CollisionPoint = GetPixelCollisionPoint(_Actor, _Renderer, FVector::DOWN);

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		if (true == IsOnGround(CollisionPoint))
		{
			Knight->SetOnGround(true);
		}
		else
		{
			Knight->SetOnGround(false);
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (true == IsOnGround(CollisionPoint))
		{
			Monster->SetOnGround(true);
		}
		else
		{
			Monster->SetOnGround(false);
		}
	}

	Gravity(_Actor, DeltaTime);
}

FVector ARoom::GetPixelCollisionPoint(AActor* _Actor, UContentsRenderer* _Renderer, FVector _Offset)
{
	float DeltaTime = UEngineCore::GetDeltaTime();

	FVector ActorPos = _Actor->GetActorLocation() - LeftTopPos;
	FVector NextPos = GravityForce * DeltaTime;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f;

	FVector CollisionPoint = { ActorPos.X + NextPos.X + _Offset.X, ActorPos.Y + NextPos.Y - HalfRendererHeight + _Offset.Y }; // 1픽셀 보정

	CollisionPoint.RoundVector();

	return CollisionPoint;
}

bool ARoom::IsOnGround(FVector _Pos)
{
	FVector CollisionPoint = _Pos;

	CollisionPoint.RoundVector();

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (CollisionColor == UColor::BLACK)
	{
		return true;
	}
	else if (CollisionColor == UColor::WHITE)
	{
		return false;
	}

	return false;
}

bool ARoom::IsOnGround(AActor* _Actor, UContentsRenderer* _Renderer, FVector _Pos)
{
	float DeltaTime = UEngineCore::GetDeltaTime();

	FVector ActorPos = _Actor->GetActorLocation() - LeftTopPos;
	FVector NextPos = GravityForce * DeltaTime;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f;

	FVector CollisionPoint = { ActorPos.X + NextPos.X, ActorPos.Y + NextPos.Y - HalfRendererHeight - 1.0f }; // 1픽셀 보정
	CollisionPoint += _Pos;
	CollisionPoint.RoundVector();

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (CollisionColor == UColor::BLACK)
	{
		return true;
	}
	else if (CollisionColor == UColor::WHITE)
	{
		return false;
	}

	return false;
}

void ARoom::Gravity(AActor* _Actor, float _DeltaTime)
{
	if (true == bActiveGravity)
	{
		return;
	}
	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		if (false == Knight->IsOnGround())
		{
			float GravityValue = 1000.0f;
			GravityForce += FVector::DOWN * GravityValue * _DeltaTime;
			Knight->AddRelativeLocation(GravityForce * _DeltaTime);
		}
		else
		{
			GravityForce = FVector::ZERO;
		}

		if (1500.0f <= GravityForce.Length())
		{
			GravityForce = FVector::DOWN * 1000.0f;
		}
		
		while (true == IsOnGround(Knight->GetActorLocation() + FVector{0.0f, 2.0f}))
		{
			Knight->AddRelativeLocation(FVector::UP);
		}
	}

	// 몬스터 로직 추가 필요
}

void ARoom::CheckPixelCollisionWithWall(AActor* _Actor, UContentsRenderer* _Renderer, float _Speed, bool _Left)
{
	if (true == bActiveGravity)
	{
		return;
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	FVector ActorPos = _Actor->GetActorLocation() - LeftTopPos;
	float HalfRendererWidth = _Renderer->GetScale().X * 0.5f;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.4f;

	FVector CollisionPoint = { ActorPos.X + NextPos , ActorPos.Y - HalfRendererHeight };

	// 왼쪽, 오른쪽 방향 구분
	if (true == _Left)
	{
		CollisionPoint.X -= HalfRendererWidth - 1.0f;
	}
	else
	{
		CollisionPoint.X += HalfRendererWidth + 1.0;
	}

	// 실수오차 문제 때문에
	CollisionPoint.X = ::roundf(CollisionPoint.X);
	CollisionPoint.Y = ::roundf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::YELLOW )
		{
			Knight->SetWallHere(true);
		}
		else
		{
			Knight->SetWallHere(false);
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::YELLOW)
		{
			//Knight->SetWallHere(true);
		}
		else
		{
			//Knight->SetWallHere(false);
		}
	}
}

void ARoom::CheckPixelCollisionWithCeil(AActor* _Actor, UContentsRenderer* _Renderer, float _Speed, bool _Left)
{
	if (true == bActiveGravity)
	{
		return;
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	FVector ActorPos = _Actor->GetActorLocation() - LeftTopPos;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f;

	FVector CollisionPoint = { ActorPos.X + NextPos , ActorPos.Y - HalfRendererHeight };

	// 실수오차 문제 때문에
	CollisionPoint.X = ::roundf(CollisionPoint.X);
	CollisionPoint.Y = ::roundf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::RED)
		{
			Knight->SetCeilHere(true);
		}
		else
		{
			Knight->SetCeilHere(false);
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::RED)
		{
			//Knight->SetWallHere(true);
		}
		else
		{
			//Knight->SetWallHere(false);
		}
	}
}



