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

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
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
	Door->SetActorLocation( _InitPos);

	Door->SetScale(_DoorScale);
	Door->SetWarpPosition(_TargetRoom, _TargetPos, _IsDoor);
	Door->GetCollision()->SetDebugColor({ 1.0f, 0.0f, 1.0f, 1.0f });
	return Door;
}

// 중력
void ARoom::CheckPixelCollisionWithGravity(AActor* _Actor, class UContentsRenderer* _Renderer, FVector _Offset)
{
	float DeltaTime = UEngineCore::GetDeltaTime();

	FVector CollisionPoint = GetPixelCollisionPoint(_Actor, _Renderer, _Offset);

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
			UEngineDebug::OutPutString("몬스터 중력 Pos : " + CollisionPoint.ToString());
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

	FVector GravityForce = FVector::ZERO;
	FVector CollisionPos = FVector::ZERO;
	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		CollisionPos = Knight->GetPixelCollision()->GetWorldLocation();
		GravityForce = Knight->GetGravityForce();
	}
	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		CollisionPos = Monster->GetPixelCollision()->GetWorldLocation();
		GravityForce = Monster->GetGravityForce();
	}

	CollisionPos -= LeftTopPos;
	FVector NextPos = GravityForce * DeltaTime;

	FVector CollisionPoint = { CollisionPos.X + NextPos.X + _Offset.X, CollisionPos.Y + NextPos.Y + _Offset.Y };

	CollisionPoint.RoundVector();

	return CollisionPoint;
}

bool ARoom::IsOnGround(FVector _Pos)
{
	FVector CollisionPoint = _Pos;

	CollisionPoint.RoundVector();

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	if (CollisionColor == UColor::BLACK || CollisionColor == UColor(0, 0, 0, 0))
	{
		return true;
	}
	else
	{
		return false;
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
		FVector GravityForce = Knight->GetGravityForce();
		if (false == Knight->IsOnGround())
		{
			//float DeltaTime = UEngineCore::GetDeltaTime();
			//FVector NextPos = GravityForce * DeltaTime;
			//FVector Pos = Knight->GetActorLocation() + Knight->GetRenderer()->GetScale() * 0.5f - LeftTopPos + NextPos + FVector(0.0f, -1.0f, 0.0f);
			//while (true == IsOnGround(Pos))
			//{
			//	Knight->AddRelativeLocation(FVector::UP);
			//}
			float GravityValue = 2000.0f;
			GravityForce += FVector::DOWN * GravityValue * _DeltaTime;
			Knight->AddRelativeLocation(GravityForce * _DeltaTime);
		}
		else
		{
			GravityForce = FVector::ZERO;
		}

		Knight->SetGravityForce(GravityForce);
		if (1500.0f <= GravityForce.Length())
		{
			GravityForce = FVector::DOWN * 1000.0f;
		}
	}
	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		FVector GravityForce = Monster->GetGravityForce();
		if (false == Monster->IsOnGround())
		{
			float GravityValue = 2000.0f;
			GravityForce += FVector::DOWN * GravityValue * _DeltaTime;
			Monster->AddRelativeLocation(GravityForce * _DeltaTime);
		}
		else
		{
			GravityForce = FVector::ZERO;
		}

		Monster->SetGravityForce(GravityForce);
		if (1500.0f <= GravityForce.Length())
		{
			GravityForce = FVector::DOWN * 1000.0f;
		}
	}
}

void ARoom::CheckPixelCollisionWithWall(AActor* _Actor, UContentsRenderer* _Renderer, float _Speed, bool _Left, FVector _Offset)
{
	if (true == bActiveGravity)
	{
		return;
	}

	FVector CollisionPos = FVector::ZERO;
	FVector CollisionHalfScale = FVector::ZERO;

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);
	if (nullptr != Knight)
	{
		CollisionPos = Knight->GetPixelCollision()->GetWorldLocation();
		CollisionHalfScale = Knight->GetPixelCollision()->GetWorldScale3D().Half();
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		CollisionPos = Monster->GetPixelCollision()->GetWorldLocation();
		CollisionHalfScale = Monster->GetPixelCollision()->GetWorldScale3D().Half();
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	CollisionPos -= LeftTopPos;

	FVector CollisionPoint = { CollisionPos.X + NextPos , CollisionPos.Y + 20.0f};

	// 왼쪽, 오른쪽 방향 구분
	if (true == _Left)
	{
		CollisionPoint.X -= CollisionHalfScale.X;
	}
	else
	{
		CollisionPoint.X += CollisionHalfScale.X;
	}

	// 실수오차 문제 때문에
	CollisionPoint.X = ::roundf(CollisionPoint.X);
	CollisionPoint.Y = ::roundf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전
	std::string R = std::to_string(CollisionColor.R);
	std::string G = std::to_string(CollisionColor.G);
	std::string B = std::to_string(CollisionColor.B);
	std::string Result = "R : " + R + " " + "G : " + G + " " + "B : " + B;

	if (nullptr != Knight)
	{
		//UEngineDebug::OutPutString("나이트 Pixel Pos : " + CollisionPoint.ToString() + " Pixel Color : " + Result);
		if (CollisionColor == UColor::YELLOW || CollisionColor == UColor::BLACK || CollisionColor == UColor::RED)
		{
			Knight->SetWallHere(true);
		}
		else
		{
			Knight->SetWallHere(false);
		}
	}

	if (nullptr != Monster)
	{
		//UEngineDebug::OutPutString("몬스터 벽 Pixel Pos : " + CollisionPoint.ToString() + " Pixel Color : " + Result);
		if (CollisionColor == UColor::YELLOW || CollisionColor == UColor::BLACK || CollisionColor == UColor::RED)
		{
			Monster->SetWallHere(true);
		}
		else if (CollisionColor == UColor::WHITE)
		{
			Monster->SetWallHere(false);
		}
		else
		{
			Monster->SetWallHere(true);
		}
	}
}

void ARoom::CheckPixelCollisionWithCeil(AActor* _Actor, UContentsRenderer* _Renderer, float _Speed, bool _Left, FVector _Offset)
{
	if (true == bActiveGravity)
	{
		return;
	}

	float DeltaTime = UEngineCore::GetDeltaTime();

	float NextPos = _Speed * DeltaTime;
	FVector ActorPos = _Actor->GetActorLocation() - LeftTopPos;
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f + _Offset.Y;

	FVector CollisionPoint = { ActorPos.X + NextPos , ActorPos.Y + HalfRendererHeight };

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
			Monster->SetCeilHere(true);
		}
		else
		{
			Monster->SetCeilHere(false);
		}
	}
}



