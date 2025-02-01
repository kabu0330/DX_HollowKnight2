#include "PreCompile.h"
#include "Monster.h"
#include "FightUnit.h"
#include <EngineBase/EngineRandom.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"

AMonster::AMonster()
{
	SetName("Monster");
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// Renderer
	ZSort = static_cast<float>(EZOrder::MONSTER);
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

void AMonster::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 150.0f;
	Data.InitVelocity = Data.Velocity;
	Data.RunSpeed = Data.Velocity * 2.5f;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 20;
	Data.Hp = 5;
	Data.MaxMp = 0;
	Data.Mp = 0;
	Data.Att = 1;
	Data.SpellAtt = 2;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 200.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = InitJumpForce;
	bCanRotation = true; // 기본 회전 가능
	bCanJump = true; // 점프하는 몬스터만 true
	bIsAggressive = true; // 호전적이면 true

}

void AMonster::SetOffset()
{
	RendererOffset = { 0.0f, 0.0f };
	BodyCollisionOffset = { 0.0f, 0.0f };
	//GravityPointOffset.Y = 1973.0f / 2.0f; // (이미지 크기 - 1프레임 크기) / 2.0f
	WallPointOffest = { -1394.0f / 2.0f, GravityPointOffset.Y - 130.0f };

	DetectRange = { 700, 50 };
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	Knight = AKnight::GetPawn();

	SetOffset();
	SetStatus();
	CreateAnimation();
	CreateCollision();
	CreateCenterPoint();
	CreateDetectCollision();
	CreatePixelCollision();
	SetCollisionEvent(); 
	SetFSM();

	InitBodyCollisionOffset = BodyCollisionOffset;
	BodyRenderer->SetRelativeLocation({ RendererOffset.X, RendererOffset.Y, ZSort });
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CheckCurRoom(); // 현재 나이트가 속한 룸 위치를 계속 체크
	SetPause(); // 나이트가 몬스터가 속한 룸과 일치하지 않으면 bIsPause로 정지
	ActivePixelCollision(); // 픽셀 충돌

	Knockback(_DeltaTime);
	ReverseForce(_DeltaTime);

	TimeElapsed(_DeltaTime); // 쿨타임 계산

	UpdateFSM(_DeltaTime);

	DebugInput(_DeltaTime);
}

void AMonster::TimeElapsed(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}

	if (true == bCanMove) // 이동 쿨타임
	{
		if (false == bIsChasing) // 플레이어 추적 상태가 아닐 때만 이동 쿨타임 적용
		{
			MoveElapsed += _DeltaTime;
			if (MoveElapsed >= MoveDuration)
			{
				bCanMove = false;
				MoveElapsed = 0.0f;

				TimeEventer->AddEndEvent(MoveCooldown, [this]()
					{
						bCanMove = true;
						bChooseDirection = false; // 방향 랜덤 결정
					});
			}
		}
	}

	if (true == bIsChasing) // 추적 방향 무한 좌우반전 제한
	{
		if (true == bIsChangeChasingDir)
		{
			ChasingDirectionElapsed += _DeltaTime;
			if (ChasingDirectionElapsed >= ChasingDirectonCooldown)
			{
				ChasingDirectionElapsed = 0.0f;
				bIsChangeChasingDir = false;
			}
		}
	}

	if (false == bCanAttack) // 공격 쿨타임 적용
	{
		AttackElapsed += _DeltaTime;
		if (AttackElapsed >= AttackCooldown)
		{
			AttackElapsed = 0.0f;
			bCanAttack = true;
		}
	}

}

void AMonster::ResetAttackCooldown()
{
	bCanAttack = true;
	AttackElapsed = 0.0f;
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
	if (true == bIsWallHere)
	{
		return;
	}		
	CheckDirection(); // 좌우 반전 적용
	float DeltaTime = UEngineCore::GetDeltaTime();
	AddActorLocation(Direction * Stat.GetRunSpeed() * DeltaTime);
}

void AMonster::Jump(float _DeltaTime)
{
	if (true == bIsCeilHere)
	{
		return;
	}

	CheckDirection(); // 좌우 반전 적용
	JumpForce = InitJumpForce;
	float JumpForceMax = 1800.0f;
	if (JumpForce >= JumpForceMax)
	{
		JumpForce = JumpForceMax;
	}
	AddActorLocation({ 0.0f, JumpForce * _DeltaTime});
	UEngineDebug::OutPutString("몬스터 점프 포스 : " + std::to_string(JumpForce));
	TimeEventer->AddEndEvent(3.0f, [this]()
		{
			JumpForce = 0.0f;
		});
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
	if (true == bIsChangeChasingDir)
	{
		return FVector::ZERO;
	}
	UEngineDebug::OutPutString("플레이어 추적");

	bIsChangeChasingDir = true; // 일정 주기마다 들어오도록 방어


	CheckDirectionToPlayer();

	return Direction;
}

void AMonster::CheckDirectionToPlayer()
{
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
}

FVector AMonster::GetRandomDirection()
{
	if (false == bIsFirstIdle)
	{
		return FVector::ZERO;
	}
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
	else // 날 수 있으면 6방향
	{
		Result = Random.RandomInt(0, 5);
	}

	switch (Result)
	{
	case 0:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 왼쪽");
		Direction = FVector::LEFT;
		bIsLeft = true;
		break;
	}
	case 1:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 오른쪽");
		Direction = FVector::RIGHT;
		bIsLeft = false;
		break;
	}
	case 2:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 왼쪽 위");
		Direction = LeftTop;
		bIsLeft = true;
		break;
	}
	case 3:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 왼쪽 아래");
		Direction = LeftBot;
		bIsLeft = true;
		break;
	}
	case 4:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 오른쪽 위");
		Direction = RightTop;
		bIsLeft = false;
		break;
	}
	case 5:
	{
		UEngineDebug::OutPutString("GetRandomDirection() : 오른쪽 아래");
		Direction = RightBot;
		bIsLeft = false;
		break;
	}
	default:
		break;
	}
	bIsFirstIdle = false;
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

	if (50.0f >= Stat.GetKnockbackForce().Length())
	{
		Stat.SetKnockbackDir(FVector::ZERO);
	}

	if (true == bIsWallHere)
	{
		return;
	}

	Stat.AddKnockbackForce(Reverse * _DeltaTime * 500.0f);
}

void AMonster::Knockback(float _DeltaTime)
{
	if (true == IsPause())
	{
		return;
	}
	if (true == Stat.IsDeath())
	{
		return;
	}
	if (true == bIsWallHere)
	{
		return;
	}
	if (FVector::ZERO != Stat.GetKnockbackForce())
	{
		AddActorLocation(Stat.GetKnockbackForce() * _DeltaTime);
		FVector Result = Stat.GetKnockbackForce() * _DeltaTime;
		int a = 0;
	}
}

void AMonster::DeathAir(float _DeltaTime)
{
	// 1. 밀려날 방향
	FVector KnightPos = Knight->GetActorLocation();
	FVector MonsterPos = GetActorLocation();
	FVector Direction = MonsterPos - KnightPos;
	Direction.Normalize();

	// 2. 포물선으로 날아가도록 로직 추가 
	Direction += FVector::UP;
	Direction.Normalize();

	// 3. 넉백 값 입력
	Stat.SetKnockbackDir(Direction);
	Stat.SetKnockbackDistance(DeathAirDistance);

	// 4. 중력 및 튕김 로직 추가
	if (false == bIsOnGround)
	{
		// 중력이 적용된 넉백
		FVector CurrentForce = Stat.GetKnockbackForce();
		CurrentForce += GravityForce;

		// 최대 힘 제한
		if (CurrentForce.Y < -3000.0f) // 중력에 의한 속도 제한
		{
			CurrentForce.Y = -3000.0f;
		}
		Stat.SetKnockbackForce(CurrentForce);
		AddActorLocation(CurrentForce * _DeltaTime);
	}
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
	bChooseDirection = true; // true면 방향 그만 바꿔
	FVector FinalVelocity = FVector::ZERO;
	if (false == bCanFly)
	{
		FinalVelocity = FVector(Stat.GetVelocity() * _DeltaTime, 0.0f);
	}
	else // 하늘을 나는 몬스터는 y축도 이동할 수 있다.
	{
		FinalVelocity = FVector(Stat.GetVelocity() * _DeltaTime, Stat.GetVelocity() * _DeltaTime * 0.6f);
	}

	FinalVelocity *= Direction;

	AddActorLocation(FinalVelocity);
}

void AMonster::DebugInput(float _DeltaTime)
{
	if (UEngineInput::IsDown(VK_F3))
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
	if (true == APlayGameMode::GetPauseRef()) // 정지상태면 
	{
		return true;
	}
	if (true == bIsPause)
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
		//bIsTurn = true;
	}

	if (false == bIsFlip)
	{
		if (bIsLeft == true)
		{
			SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
			BodyCollisionOffset.X = InitBodyCollisionOffset.X;
		}
		if (bIsLeft == false)
		{
			SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
			int Offset = BodyCollisionOffset.X * 1.5f;
			BodyCollisionOffset.X = Offset;
		}
	}

	// 좌우반전
	if (true == bIsFlip)
	{
		if (bIsLeft == true)
		{
			SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
		}
		if (bIsLeft == false)
		{
			SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
		}
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

