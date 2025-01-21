#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"
#include <EngineBase/EngineRandom.h>

AMonster::AMonster()
{
	SetName("��Ȳ�ϴ� ������");
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	TimeEventor = CreateDefaultSubObject<UTimeEventComponent>().get();

	ZSort = static_cast<float>(EZOrder::MONSTER);
	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	GravityPointOffset.Y = 1973.0f / 2.0f; // (�̹��� ũ�� - 1������ ũ��) / 2.0f
	WallPointOffest = { -1394.0f / 2.0f, GravityPointOffset.Y }; // 

	DetectRange = { 300, 300 };

	CreateAnimation();
	CreateCollision();
	CreateCenterPoint();
	CreateDetectCollision();

	SetCollisionEvent();
	SetFSM();
	SetStatus();
}

void AMonster::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 50.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 1.3f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 20;
	Data.Hp = 20;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 50.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = InitJumpForce;
	bCanRotation = true;
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	Knight = AKnight::GetPawn();
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CheckCurRoom(); // ���� ����Ʈ�� ���� �� ��ġ�� ��� üũ
	SetPause(); // ����Ʈ�� ���Ͱ� ���� ��� ��ġ���� ������ bIsPause�� ����
	ActivePixelCollision();

	Move(_DeltaTime);
	TimeElapsed(_DeltaTime);

	FSM.Update(_DeltaTime);
	DebugInput(_DeltaTime);

	// Test
	//IsPlayerNearby();
	//GetDirectionToPlayer();
	CheckDirection();
}

bool AMonster::IsPlayerNearby()
{
	if (false == IsCurRoom()) // �÷��̾ �ش� �ʿ� ���ٸ� ����
	{
		return false;
	}
	if (nullptr == DetectCollision) // Ž�� ������ ������ ����
	{
		return false;
	}

	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = this->GetActorLocation();
	FVector Distance = KnightPos - MonsterPos;

	float Length = Distance.Length();
	float DetectLength = DetectCollision->GetWorldScale3D().Half().Length();

	if (DetectLength >= Length)
	{
		return true;
	}
	return false;
}

FVector AMonster::GetDirectionToPlayer()
{
	if (false == IsCurRoom()) // �÷��̾ �ش� �ʿ� ���ٸ� ����
	{
		return FVector::ZERO;
	}
	if (nullptr == Knight)
	{
		return FVector::ZERO;
	}
	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = this->GetActorLocation();
	FVector Distance = KnightPos - MonsterPos;

	Distance.Normalize();

	if (Distance.X <= 0)
	{
		bIsLeft = true;
	}
	else if (Distance.X > 0)
	{
		bIsLeft = false;
	}

	return Distance;
}

FVector AMonster::GetRandomDirection()
{
	FVector LeftTop = FVector::LEFT + FVector::UP;
	LeftTop.Normalize();

	FVector LeftBot = FVector::LEFT + FVector::DOWN;
	LeftBot.Normalize();

	FVector RightTop = FVector::RIGHT + FVector::UP;
	RightTop.Normalize();

	FVector RightBot = FVector::RIGHT + FVector::DOWN;
	RightBot.Normalize();

	UEngineRandom Random;
	int Result = -1;
	if (false == bCanFly) // �� �� ���� �ִ� �¿� �̵���
	{
		Result = Random.RandomInt(0, 1);
	}
	else // �� �� ������ 8����
	{
		Result = Random.RandomInt(0, 7);
	}

	switch (Result)
	{
	case 0:
	{
		Direction = FVector::LEFT;
		break;
	}
	case 1:
	{
		Direction = FVector::RIGHT;
		break;
	}
	default:
		break;
	}
	


	return Direction;
}

void AMonster::Move(float _DeltaTime)
{
	if (false == IsCurRoom())
	{
		return;
	}
	if (true == bIsWallHere)
	{
		return;
	}
	if (false == bCanMove)
	{
		return;
	}

	FVector FinalVelocity = FVector(Stat.GetVelocity() * _DeltaTime, 0.0f);
	FinalVelocity *= Direction;

	AddActorLocation(FinalVelocity);
}

void AMonster::TimeElapsed(float _DeltaTime)
{
	if (false == IsCurRoom())
	{
		return;
	}

	if (true == bCanMove)
	{
		MoveElapsed += _DeltaTime;
		if (MoveElapsed >= MoveDuration)
		{
			bCanMove = false;
			MoveElapsed = 0.0f;

			float MoveCooldown = 2.0f;
			TimeEventor->AddEndEvent(MoveCooldown, [this]()
				{
					bCanMove = true;
				});
		}
	}
}

void AMonster::DebugInput(float _DeltaTime)
{
}

bool AMonster::IsCurRoom()
{
	if (CurRoom == ParentRoom)
	{
		return true;
	}
	return false;
}

bool AMonster::CanAction()
{
	if (true == bIsPause)
	{
		return false;
	}
	if (true == bIsDeath)
	{
		return false;
	}
	if (true == bIsAttacking)
	{
		return false;
	}
	if (true == bIsStun)
	{
		return false;
	}

	return true;
}

bool AMonster::CanJump()
{
	if (false == CanAction())
	{
		return false;
	}
	if (false == bCanJump)
	{
		return false;
	}

	return true;
}

void AMonster::CheckDirection()
{
	if (false == CanAction())
	{
		return;
	}
	if (false == bCanRotation)
	{
		return;
	}
	if (bIsLeft == true)
	{
		SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
	}
	if (bIsLeft == false)
	{
		SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
	}
}

void AMonster::SetPause()
{
	if (true == IsCurRoom())
	{
		bIsPause = false;
	}
	else
	{
		bIsPause = true;
	}
}

void AMonster::CheckCurRoom()
{
	ARoom* MainPawnRoom = ARoom::GetCurRoom();
	CurRoom = MainPawnRoom;
}

