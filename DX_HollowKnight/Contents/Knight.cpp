#include "PreCompile.h"
#include "Knight.h"

#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/Collision.h>

#include "KnightSkill.h"
#include "Room.h"
#include "PlayGameMode.h"
#include "Door.h"
#include <math.h>

AKnight* AKnight::MainPawn = nullptr;

AKnight::AKnight()
{
	SetName("AKnight");
	MainPawn = this;

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
	TimeEventor = CreateDefaultSubObject<UTimeEventComponent>().get();

	CreateRenderer();
	CreateCollision();
	SetStatus();

	//SetActorLocation({ 1100.0f, -3000.0f });
	//SetActorLocation(InitPos::Dirtmouth_well);
	//SetActorLocation(InitPos::CrossroadsEntrance);
	//SetActorLocation({9911, -5500});
	SetActorLocation({1100, -2980});

	// Debug
	BodyRenderer->BillboardOn();

}

void AKnight::BeginPlay()
{
	AActor::BeginPlay();
	SetFSM();
	SetCollisionEvent();
	SetCameraPos();
}

void AKnight::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	ActivePixelCollsion();

	CheckCameraPos();
	SetCameraLerp();
	CheckEnterButton();

	FSM.Update(_DeltaTime);

	TimeElapsed(_DeltaTime);

	DebugInput(_DeltaTime);
}

void AKnight::SetCollisionEvent()
{
	BodyCollision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			//_Other->GetActor()->Destroy();
			// _Other->Destroy();
			UEngineDebug::OutPutString("Kinigt Collision Event Enter");
		});

	BodyCollision->SetCollisionStay(std::bind(&AKnight::CheckEnterDoor, this, std::placeholders::_1, std::placeholders::_2));
}

void AKnight::ActiveGravity()
 {
	if (true == NoneGravity)
	{
		return;
	}

	ARoom* Room = ARoom::GetCurRoom();
	if (nullptr != Room)
	{
		Room->CheckPixelCollisionWithGravity(this, BodyRenderer.get());
	}
}

void AKnight::ActivePixelCollsion()
{
	if (true == NoneGravity)
	{
		return;
	}
	ARoom* Room = ARoom::GetCurRoom();
	if (nullptr != Room)
	{
		Room->CheckPixelCollisionWithWall(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft);
		Room->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Stat.GetVelocity(), bIsLeft);
	}
}

void AKnight::SetStatus()
{
	FStatusData Data;
	Data.Velocity = 500.0f;
	Data.InitVelocity = Data.Velocity;
	Data.DashSpeed = Data.Velocity * 3.0f;
	Data.MaxHp = 5;
	Data.Hp = 5;
	Data.MaxMp = 99;
	Data.Mp = 0;
	Data.Att = 5;
	Data.SpellAtt = 15;
	Data.bIsKnockbackable = true;
	Data.KnockbackDistance = 50.0f;
	Data.Geo = 0;
	Stat.CreateStatus(&Data);

	JumpForce = InitJumpForce;
	bCanRotation = true;
}

void AKnight::Move(float _DeltaTime)
{

	if (false == bIsDashing)
	{
		Stat.SetVelocity(Stat.GetInitVelocity());

	}
	if (UEngineInput::IsDown('C'))
	{
		Stat.SetVelocity(Stat.GetDashSpeed());
		//Velocity = DashSpeed;
	}
	if (true == bIsDashing)
	{
		return;
	}

	CheckDirection();

	if (UEngineInput::IsPress(VK_LEFT))
	{
		if (false == bIsWallHere)
		{
			//AddRelativeLocation(FVector{ -Velocity * _DeltaTime, 0.0f, 0.0f });
			AddRelativeLocation(FVector{ -Stat.GetVelocity() * _DeltaTime, 0.0f, 0.0f });
		}

	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		if (false == bIsWallHere)
		{
			//AddRelativeLocation(FVector{ Velocity * _DeltaTime, 0.0f, 0.0f });
			AddRelativeLocation(FVector{ Stat.GetVelocity() * _DeltaTime, 0.0f, 0.0f });
		}
	}
}

void AKnight::SetCameraPos()
{
	bIsCameraMove = false;
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	FVector KnightPos = GetActorLocation();
	FVector ScreenSize = UEngineCore::GetScreenScale();
	ScreenRatioY = 0.25f;
	Camera->SetActorLocation({ KnightPos.X, KnightPos.Y + ScreenSize.Y * ScreenRatioY });
	FVector Pos = Camera->GetActorLocation();
}

void AKnight::CheckCameraPos()
{
	if (bIsCameraMove == true)
	{
		return;
	}

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	FVector KnightPos = GetActorLocation();
	FVector CameraPos = Camera->GetActorLocation();
	FVector ScreenSize = UEngineCore::GetScreenScale();

	float Distance = ::abs(KnightPos.Length() - CameraPos.Length());
	float Width = ::abs(KnightPos.X - CameraPos.X);
	float Height = ::abs(KnightPos.Y - (CameraPos.Y - ScreenSize.Y * ScreenRatioY));
	float KnightPosY = KnightPos.Y - BodyRenderer->GetScale().Y * 0.5f;
	float Rs = CameraPos.Y - ScreenSize.Y * ScreenRatioY;

	if (Height > 200.0f || Width > 100.0f)
	{
		CameraCurPos = CameraPos;
		
		float TargetY = KnightPos.Y + ScreenSize.Y * ScreenRatioY;
		float ClampPosY = UEngineMath::Clamp(TargetY, (CameraPos.Y - 0.0f) , (CameraPos.Y + 0.0f));
		CameraTargetPos = { KnightPos.X, ClampPosY };

		bIsCameraMove = true;
	}
}

void AKnight::SetCameraLerp()
{
	if (false == bIsCameraMove)
	{
		return;
	}

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	float CameraMoveDuration = 0.2f;
	float Alpha = CameraMoveTime / CameraMoveDuration;

	FVector CameraDest = UEngineMath::Lerp(CameraCurPos, CameraTargetPos, Alpha);
	Camera->SetActorLocation(CameraDest);


	float DeltaTime = UEngineCore::GetDeltaTime();
	CameraMoveTime += DeltaTime;
	if (CameraMoveDuration <= CameraMoveTime)
	{
		CameraMoveTime = 0.0f;
		bIsCameraMove = false;
	}

	FVector CameraResult = GetActorLocation();
}

void AKnight::TimeElapsed(float _DeltaTime)
{
	if (true == ARoom::GetActiveGravity())
	{
		return;
	}
	float AttackCooldown = 0.5f;

	if (true == bIsAttacking)
	{
		AttackCooldownElapsed += _DeltaTime;
		if (AttackCooldownElapsed >= AttackCooldown)
		{
			bIsAttacking = false;
			bIsShowEffect = false;
			AttackCooldownElapsed = 0.0f;
		}
	}

	float DashCooldown = 0.3f;
	if (false == bCanDash)
	{
		DashCooldownElapsed += _DeltaTime;
		if (DashCooldownElapsed >= DashCooldown)
		{
			if (true == bIsOnGround)
			{
				bCanDash = true;
				DashCooldownElapsed = 0.0f;
			}
		}
	}
}

void AKnight::CheckEnterButton()
{
	if (UEngineInput::IsUp(VK_UP))
	{
		bIsEnter = false;
	}
}

void AKnight::CheckEnterDoor(UCollision* _This, UCollision* _Target)
{
	AActor* TargetActor =  _Target->GetActor();
	ADoor* Door = dynamic_cast<ADoor*>(TargetActor);
	if (nullptr == Door)
	{
		return;
	}

	if (UEngineInput::IsDown(VK_UP))
	{
		bIsEnter = true;
	}
}

bool AKnight::CanAction()
{
	if (true == bIsDeath)
	{
		return false;
	}
	if (true == bIsCastingSpell)
	{
		return false;
	}
	if (true == bIsAttacking)
	{
		return false;
	}
	if (true == bIsBeingHit)
	{
		return false;
	}

	return true;
}

void AKnight::ChangeDash()
{
	if (false == CanAction())
	{
		return;
	}

	if (false == bCanDash)
	{
		return;
	}
	if (UEngineInput::IsDown('C'))
	{
		bIsDashing = true;
		FSM.ChangeState(EKnightState::DASH);
		return;
	}
}

void AKnight::Dash()
{
	CreateDashEffect();

	float DeltaTime = UEngineCore::GetDeltaTime();
	bCanRotation = false;
	bIsDashing = true;
	bCanDash = false;

	if (false == bIsWallHere)
	{
		if (true == bIsLeft)
		{
			AddRelativeLocation(FVector{ -Stat.GetVelocity() * DeltaTime, 0.0f, 0.0f });
		}
		else
		{
			AddRelativeLocation(FVector{ Stat.GetVelocity() * DeltaTime, 0.0f, 0.0f });
		}
	}
}

void AKnight::CastFocus()
{
	if (false == CanAction())
	{
		return;
	}

	if (UEngineInput::IsPress('A'))
	{
		float PressTime = UEngineInput::IsPressTime('A');
		float TriggerTime = 0.3f;

		if (PressTime >= TriggerTime)
		{
			FSM.ChangeState(EKnightState::FOCUS);
			return;
		}
	}
}


void AKnight::CastFireball()
{
	if (false == CanAction())
	{
		return;
	}
	if (UEngineInput::IsUp('A'))
	{
		FSM.ChangeState(EKnightState::FIREBALL_ANTIC);
		return;
	}
}

bool AKnight::CanJump()
{
	if (false == CanAction())
	{
		return false;
	}
	if (false == bIsOnGround)
	{
		return false;
	}

	return true;
}

void AKnight::Jump(float _DeltaTime)
{
	if (true == CanAction())
	{
		if (true == bIsCeilHere)
		{
			return;
		}
		if (UEngineInput::IsPress('Z'))
		{		
			float JumpAccTime = 0.3f;
			float JumpKeyDuration = UEngineInput::IsPressTime('Z');
			if (JumpAccTime >= JumpKeyDuration)
			{
				JumpForce = 1200.0f;

				float JumpForceMax = 1800.0f;
				if (JumpForce >= JumpForceMax)
				{
					JumpForce = JumpForceMax;
				}
			}
	
			AddRelativeLocation(FVector{ 0.0f, JumpForce * _DeltaTime, 0.0f });	
		}
	}
}

void AKnight::ChangeJumpAnimation()
{
	if (true == CanJump())
	{
		if (UEngineInput::IsPress('Z'))
		{
			FSM.ChangeState(EKnightState::JUMP);
			return;
		}
	}
}

void AKnight::ChangeNonCombatAnimation()
{
	if (true == CanAction())
	{
		return;
	}
}

void AKnight::ChangeLookAnimation()
{
	//if (UEngineInput::IsPress(VK_DOWN))
	//{
	//	float PressTime = UEngineInput::IsPressTime(VK_DOWN);
	//	float TriggerTime = 0.5f;

	//	if (PressTime >= TriggerTime)
	//	{
	//		FSM.ChangeState(EKnightState::LOOK_DOWN);
	//		return;
	//	}
	//}
	//if (UEngineInput::IsPress(VK_UP))
	//{
	//	float PressTime = UEngineInput::IsPressTime(VK_UP);
	//	float TriggerTime = 0.5f;

	//	if (PressTime >= TriggerTime)
	//	{
	//		FSM.ChangeState(EKnightState::LOOK_UP);
	//		return;
	//	}
	//}
}

void AKnight::DebugInput(float _DeltaTime)
{
	if (true == ARoom::GetActiveGravity())
	{
		bIsOnGround = true;
	}

	if (UEngineInput::IsDown('T'))
	{
		if (true == ARoom::GetActiveGravity())
		{
			//Velocity = 1500.0f;
			//InitVelocity = Velocity;
			//DashSpeed = Velocity * 3.0f;
			Stat.SetVelocity(1500.0f);
			Stat.SetInitVelocity(Stat.GetVelocity());
			Stat.SetDashSpeed(Stat.GetVelocity() * 3.0f);
			bCanDash = true;
		}		
	}

	if (UEngineInput::IsDown('V'))
	{
		FSM.ChangeState(EKnightState::STUN);
		Stat.AddHp(-1);
	}

	if (UEngineInput::IsDown('B'))
	{
		Stat.AddHp(1);
	}

	float ZValue = BodyRenderer->GetTransformRef().RelativeLocation.Z;
	int a = 0;

	if (UEngineInput::IsDown('Q'))
	{
		SwitchActiveGravity();
	}

	// Debug Input
	if (true == ARoom::GetActiveGravity())
	{
		if (UEngineInput::IsPress(VK_UP))
		{
			AddRelativeLocation(FVector{ 0.0f, Stat.GetVelocity() * _DeltaTime, 0.0f });
		}
		if (UEngineInput::IsPress(VK_DOWN))
		{
			AddRelativeLocation(FVector{ 0.0f, -Stat.GetVelocity() * _DeltaTime, 0.0f });
		}
	}
}

void AKnight::ChangeAttackAnimation(EKnightState _PrevState)
{
	if (true == CanAction())
	{
		if (UEngineInput::IsDown('X') && UEngineInput::IsPress(VK_UP))
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::UP_SLASH);
			return;
		}
		if (UEngineInput::IsDown('X') && UEngineInput::IsPress(VK_DOWN) && false == bIsOnGround)
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::DOWN_SLASH);
			return;
		}
		if (UEngineInput::IsDown('X'))
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::SLASH);
			return;
		}
	}
}

void AKnight::CreateCollision()
{
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
	BodyCollision->SetCollisionProfileName("Knight");
	BodyCollision->SetScale3D({ 100.0f, 130.0f });
	BodyCollision->GetTransformRef().Location.Y += 0.0f;
	BodyCollision->SetRelativeLocation(BodyRenderer->GetActorLocation());
	//BodyCollision->SetCollisionType(ECollisionType::AABB);

}

void AKnight::CheckDirection()
{
	if (false == CanAction()) 
	{
		return;
	}
	if (false == bCanRotation)
	{
		return;
	}
	if (UEngineInput::IsPress(VK_LEFT))
	{
		bIsLeft = true;
		SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		bIsLeft = false;
		SetActorRelativeScale3D({ -1.0f, 1.0f, 1.0f });
	}
}

AKnight::~AKnight()
{
	BodyRenderer = nullptr;
	BodyCollision = nullptr;
}
