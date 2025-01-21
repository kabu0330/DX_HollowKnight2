#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"
#include <EngineBase/EngineRandom.h>
#include <EnginePlatform/EngineInput.h>

AMonster::AMonster()
{
	SetName("방황하는 껍데기");
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	TimeEventor = CreateDefaultSubObject<UTimeEventComponent>().get();

	ZSort = static_cast<float>(EZOrder::MONSTER);
	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	GravityPointOffset.Y = 1973.0f / 2.0f - 5.f; // (이미지 크기 - 1프레임 크기) / 2.0f
	WallPointOffest = { -1394.0f / 2.0f, GravityPointOffset.Y }; // 

	DetectRange = { 500, 300 };

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
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
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
	bCanRotation = true; // 기본 회전 가능
	bCanJump = false; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true

}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	Knight = AKnight::GetPawn();
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CheckCurRoom(); // 현재 나이트가 속한 룸 위치를 계속 체크
	SetPause(); // 나이트가 몬스터가 속한 룸과 일치하지 않으면 bIsPause로 정지
	ActivePixelCollision(); // 픽셀 충돌

	ReverseForce(_DeltaTime);
	Knockback(_DeltaTime);

	TimeElapsed(_DeltaTime); // 쿨타임 계산

	FSM.Update(_DeltaTime);

	DebugInput(_DeltaTime);
}

bool AMonster::IsPlayerNearby()
{
	if (true == IsPause()) 
	{
		return false;
	}
	if (false == bIsAggressive)
	{
		return false;
	}
	if (nullptr == DetectCollision) // 탐색 범위가 없으면 리턴
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


void AMonster::Dash()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	AddActorLocation(Direction * Stat.GetRunSpeed() * DeltaTime);
}

FVector AMonster::GetDirectionToPlayer()
{
	if (true == IsPause())
	{
		return FVector::ZERO;
	}
	if (false == bIsChasing)
	{
		return FVector::ZERO;
	}
	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = this->GetActorLocation();

	FVector Distance = KnightPos - MonsterPos;
	Distance.Z = 0.0f;
	if (false == bCanFly)
	{
		Distance.Y = 0.0f;

	}
	Distance.Normalize();
	Direction = Distance;


	if (Direction.X <= 0)
	{
		bIsLeft = true;
	}
	else if (Direction.X > 0)
	{
		bIsLeft = false;
	}

	return Direction;
}

FVector AMonster::GetRandomDirection()
{
	if (true == IsPause())
	{
		return FVector::ZERO;
	}
	if (true == bIsChasing)
	{
		return FVector::ZERO;
	}
	if (true == bChooseDirection)
	{
		return FVector::ZERO; // 다음 이동까지 방향 결정하지마.
	}
	if (true == IsPlayerNearby()) // 플레이어가 근처에 있으면 랜덤 방향 결정하지마.
	{
		return FVector::ZERO;
	}
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
	if (false == bCanFly) // 날 수 없는 애는 좌우 이동만
	{
		Result = Random.RandomInt(0, 1);
	}
	else // 날 수 있으면 8방향
	{
		Result = Random.RandomInt(0, 7);
	}

	switch (Result)
	{
	case 0:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 왼쪽 이동");
		Direction = FVector::LEFT;
		bIsLeft = true;
		break;
	}
	case 1:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 오른쪽 이동");
		Direction = FVector::RIGHT;
		bIsLeft = false;
		break;
	}
	default:
		break;
	}
	
	return Direction;
}

void AMonster::CheckDeath()
{
	if (true == IsPause())
	{
		return;
	}
	if (0 >= Stat.GetHp())
	{
		FSM.ChangeState(EMonsterState::DEATH_AIR);
	}
}

void AMonster::ReverseForce(float _DeltaTime)
{
	if (FVector::ZERO == Stat.GetKnockbackForce())
	{
		return;
	}

	FVector Reverse = -Stat.GetKnockbackForce();
	Reverse.Normalize();

	Stat.AddKnockbackForce(Reverse * _DeltaTime * 500.0f);

	if (50.0f >= Stat.GetKnockbackForce().Length())
	{
		Stat.SetKnockbackDir(FVector::ZERO);
	}
}

void AMonster::Knockback(float _DeltaTime)
{
	if (FVector::ZERO != Stat.GetKnockbackForce())
	{
		AddActorLocation(Stat.GetKnockbackForce() * _DeltaTime);
	}
}

void AMonster::DeathAir(float _DeltaTime)
{
}

void AMonster::Death(float _DeltaTime)
{
}

void AMonster::Move(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}
	if (true == bIsWallHere)
	{
		return;
	}
	if (true == Stat.IsAttacking())
	{
		return;
	}
	FVector FinalVelocity = FVector(Stat.GetVelocity() * _DeltaTime, 0.0f);
	FinalVelocity *= Direction;

	AddActorLocation(FinalVelocity);
}

void AMonster::TimeElapsed(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}

	if (true == bCanMove)
	{
		if (false == bIsChasing) // 플레이어 추적 상태가 아닐 때만 이동 쿨타임 적용
		{
			bChooseDirection = true; // true면 방향 그만 바꿔
			MoveElapsed += _DeltaTime;
			if (MoveElapsed >= MoveDuration)
			{
				bCanMove = false;
				MoveElapsed = 0.0f;

				float MoveCooldown = 3.0f;
				TimeEventor->AddEndEvent(MoveCooldown, [this]()
					{
						bCanMove = true;
						bChooseDirection = false; // 방향 랜덤 결정
					});
			}
		}
	}

	if (false == bCanAttack)
	{
		AttackElapsed += _DeltaTime;
		if (AttackElapsed >= AttackDuation)
		{
			AttackElapsed = 0.0f;
			bCanAttack = true;
		}
	}
}

void AMonster::DebugInput(float _DeltaTime)
{
	if (UEngineInput::IsDown('R'))
	{
		SwitchDebugPause();
	}
}

bool AMonster::IsCurRoom()
{
	if (CurRoom == ParentRoom)
	{
		return true;
	}
	return false;
}

bool AMonster::IsPause()
{
	if (false == IsCurRoom()) // 플레이어가 다른 맵에 있으면 행동불가
	{
		return true;
	}
	if (true == bIsPause) // 정지상태면 
	{
		return true;
	}
	if (true == bIsDebugPause)
	{
		return true;
	}
	if (nullptr == Knight)
	{
		UEngineDebug::OutPutString("심각 : Moster 객체가 나이트를 인식하지 못함");
		return true;
	}
	return false;
}

// 일시정지거나, 죽었거나, 공격 중이거나, 스턴 상태면 true
bool AMonster::CanAction()
{
	if (true == IsPause()) // 현재 몬스터가 정지 상태면
	{
		return false;
	}
	if (true == Stat.IsDeath()) // 죽었으면
	{
		return false;
	}
	if (true == Stat.IsAttacking()) // 공격중이면
	{
		return false;
	}
	if (true == Stat.IsStun())
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
	if (false == bCanRotation)
	{
		return;
	}
	if (bIsPrevLeft != bIsLeft)
	{
		bIsPrevLeft = bIsLeft;
		bIsTurn = true;
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

