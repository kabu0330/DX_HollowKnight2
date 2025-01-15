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

	bActiveGravity = true;
}

ARoom::~ARoom()
{
}

void ARoom::BeginPlay()
{
	AActor::BeginPlay();

	//SetActorLocation(InitPos);
	FVector Result = GetActorLocation();
	int a = 0;
 }

void ARoom::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);
	BackgroundRenderer->SetZSort(ZSort);

	FVector Result = GetActorLocation();
}

bool ARoom::IsLinking(ARoom* _Room)
{
	for (ARoom* Room : Rooms)
	{
		if (Room == _Room)
		{
			return true;
		}
	}
	return false;
}

bool ARoom::InterLinkRoom(ARoom* _Room, FVector _OffsetPos)
{
	this->LinkRoom(_Room);
	_Room->LinkRoom(this);
	SetPixelCollisionCheckPosition(_Room, _OffsetPos);

	return true;
}

void ARoom::SetPixelCollisionCheckPosition(ARoom* _Room, FVector _OffsetPos)
{
	_Room->SetActorLocation(this->GetActorLocation() + _OffsetPos);
	_Room->SetLeftTopPos(_Room->GetActorLocation());
}

ARoom* ARoom::LinkRoom(ARoom* _Room)
{
	if (_Room == this)
	{
		MSGASSERT("자기 자신을 연결할 수 없습니다.");
		return nullptr;
	}
	if (true == IsLinking(_Room))
	{
		MSGASSERT("이미 연결된 맵입니다.");
		return nullptr;
	}
	
	Rooms.push_back(_Room);
	return Rooms[Rooms.size() - 1];
}

void ARoom::CreateTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);

	BackgroundRenderer->SetTexture(_FileName, true, _ScaleRatio);
	BackgroundRenderer->SetRelativeLocation({ Size.X / 2.0f * _ScaleRatio, -Size.Y / 2.0f * _ScaleRatio, ZSort });
}

void ARoom::CreatePixelCollisionTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::PIXELCOLLISION);

	//PixelCollisionTexture->SetTexture(_FileName, true, _ScaleRatio);
	//PixelCollisionTexture->SetRelativeLocation({ Size.X / 2.0f * _ScaleRatio, -Size.Y / 2.0f * _ScaleRatio, ZSort });
	//PixelCollisionTexture->SetRelativeLocation({Pos.X, Pos.Y, ZSort });
}

// 좌상단을 0, 0 기준으로 변경
ADoor* ARoom::CreateDoor(FVector _InitPos, ARoom* _TargetRoom, FVector _TargetPos, bool _IsEnter)
{
	ADoor* Door = GetWorld()->SpawnActor<ADoor>().get();

	// 이미지 정 가운데를 {0, 0} 기준. 좌상단 기준 {-width / 2, height / 2} 
	FVector RoomPos = { GetActorLocation().X - (Size.X / 2.0f), GetActorLocation().Y + (Size.Y / 2.0f) };
	//FVector RoomPos = { GetActorLocation().X, GetActorLocation().Y};
	Door->SetActorLocation(RoomPos + _InitPos);

	FVector InitPos = RoomPos + _InitPos;

	Door->SetWarpPosition(InitPos, _TargetRoom, _TargetPos, _IsEnter);

	return Door;
}

// 중력
void ARoom::CheckPixelCollisionWithGravity(AActor* _Actor, class UContentsRenderer* _Renderer)
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	Gravity(_Actor, DeltaTime);

	FVector NextPos = GravityForce * DeltaTime;
	FVector ActorPos = LeftTopPos - _Actor->GetActorLocation();
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f;

	FVector CollisionPoint = { ActorPos.X + NextPos.X, ActorPos.Y +  NextPos.Y - HalfRendererHeight };

	// 실수오차 문제 때문에
	CollisionPoint.X = floorf(CollisionPoint.X);
	CollisionPoint.Y = floorf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);

	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::BLACK )
		{
			Knight->SetOnGround(true);
			return;
		}
		else if (CollisionColor == UColor::WHITE)
		{
			Knight->SetOnGround(false);
			return;
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::BLACK )
		{
			Monster->SetOnGround(true);
			return;
		}
		else if (CollisionColor == UColor::WHITE )
		{
			Monster->SetOnGround(false);
			return;
		}
	}
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
	}

	if (2000.0f <= GravityForce.Length())
	{
		GravityForce.Y = 1000.0f;
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
	BlockByWall(_Actor, _Speed, DeltaTime);

	float NextPos = _Speed * DeltaTime;
	FVector ActorPos = LeftTopPos - _Actor->GetActorLocation();
	float HalfRendererWidth = _Renderer->GetScale().X * 0.5f;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.4f;

	FVector CollisionPoint = { ActorPos.X + NextPos , ActorPos.Y - HalfRendererHeight };

	// 왼쪽, 오른쪽 방향 구분
	if (true == _Left)
	{
		CollisionPoint.X -= HalfRendererWidth;
	}
	else
	{
		CollisionPoint.X += HalfRendererWidth;
	}

	// 실수오차 문제 때문에
	CollisionPoint.X = floorf(CollisionPoint.X);
	CollisionPoint.Y = floorf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);

	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::YELLOW )
		{
			Knight->SetWallHere(true);
			return;
		}
		else if (CollisionColor == UColor::WHITE)
		{
			Knight->SetWallHere(false);
			return;
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::YELLOW)
		{
			//Monster->SetOnGround(true);
			return;
		}
		else if (CollisionColor == UColor::WHITE)
		{
			//Monster->SetOnGround(false);
			return;
		}
	}
}

void ARoom::BlockByWall(AActor* _Actor, float _Speed, float _DeltaTime)
{
	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	FVector PushValue = FVector::ZERO;
	if (nullptr != Knight)
	{
		if (false == Knight->IsWallHere())
		{
			return;
		}

		if (true == Knight->IsLeft()) // 왼쪽에 벽이 있으면
		{
			Knight->SetActorLocation({ Knight->GetPrevPos().X, Knight->GetActorLocation().Y});
			//PushValue = FVector::RIGHT * _Speed * 100.0f * _DeltaTime;
			//Knight->AddRelativeLocation(PushValue * _DeltaTime);
		}
		else
		{
			Knight->SetActorLocation({ Knight->GetPrevPos().X, Knight->GetActorLocation().Y });
			//PushValue = FVector::LEFT * _Speed * 100.0f * _DeltaTime;
			//Knight->AddRelativeLocation(PushValue * _DeltaTime);
		}
	}
}



