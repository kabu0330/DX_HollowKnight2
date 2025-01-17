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



AKnight* AKnight::MainPawn = nullptr;

AKnight::AKnight()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
	CreateRenderer();
	CreateCollision();

	MainPawn = this;

	Velocity = 400.0f;
	InitVelocity = Velocity;
	DashSpeed = Velocity * 4.0f;
	JumpForce = InitJumpForce;
	bCanRotation = true;

	//SetActorLocation({ 1100.0f, -3000.0f });
	SetActorLocation(InitPos::Dirtmouth_well);

	// Debug
	BodyRenderer->BillboardOn();
	//NoneGravity = true;
	if (true == NoneGravity)
	{
		Velocity = 800.0f;	
		InitVelocity = Velocity;
		DashSpeed = Velocity * 3.0f;
	}

	BodyCollision->SetCollisionStay(std::bind(&AKnight::CheckEnterDoor, this, std::placeholders::_1, std::placeholders::_2));
}

void AKnight::BeginPlay()
{
	AActor::BeginPlay();
	SetFSM();
}

void AKnight::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	ActiveWallCollsion();

	SetCameraPosition();
	FSM.Update(_DeltaTime);

	TimeElapsed(_DeltaTime);

	DebugInput(_DeltaTime);
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

void AKnight::ActiveWallCollsion()
{
	if (true == NoneGravity)
	{
		return;
	}
	ARoom* Room = ARoom::GetCurRoom();
	if (nullptr != Room)
	{
		Room->CheckPixelCollisionWithWall(this, BodyRenderer.get(), Velocity, bIsLeft);
		Room->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Velocity, bIsLeft);
	}
}

void AKnight::Move(float _DeltaTime)
{
	if (false == bIsDashing)
	{
		Velocity = InitVelocity;

	}
	if (UEngineInput::IsDown('C'))
	{
		Velocity = DashSpeed;
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
			AddRelativeLocation(FVector{ -Velocity * _DeltaTime, 0.0f, 0.0f });
		}

	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		if (false == bIsWallHere)
		{
			AddRelativeLocation(FVector{ Velocity * _DeltaTime, 0.0f, 0.0f });
		}
	}
}

void AKnight::SetCameraPosition()
{
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	FVector Pos = RootComponent->GetTransformRef().RelativeLocation;
	FVector ScreenSize = UEngineCore::GetScreenScale();
	CameraPos = { Pos.X, Pos.Y + ScreenSize.Y * 0.25f };
	//CameraPos = { Pos.X, Pos.Y };
	Camera->SetActorLocation(CameraPos);
	FVector CameraResult = GetActorLocation();
}

void AKnight::TimeElapsed(float _DeltaTime)
{
	float AttackCooldown = 0.6f;

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

	float DashCooldown = 0.4f;
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
		Door->EnterDoor();
	}
	if (UEngineInput::IsUp(VK_UP))
	{
		Door->EnterDoorEnd();
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
	float DeltaTime = UEngineCore::GetDeltaTime();
	bCanRotation = false;
	bIsDashing = true;
	bCanDash = false;

	if (false == bIsWallHere)
	{
		if (true == bIsLeft)
		{
			AddRelativeLocation(FVector{ -Velocity * DeltaTime, 0.0f, 0.0f });
		}
		else
		{
			AddRelativeLocation(FVector{ Velocity * DeltaTime, 0.0f, 0.0f });
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
		float TriggerTime = 0.5f;

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
		if (UEngineInput::IsPress('Z'))
		{		
			float JumpForceMax = 1000.0f;
			if (true == bIsCeilHere)
			{
				JumpForceMax = 0.0f;
			}
			else
			{
				JumpForceMax = 1000.0f;
			}
			float JumpAccTime = 0.4f;
			float JumpKeyDuration = UEngineInput::IsPressTime('Z');
			if (JumpAccTime >= JumpKeyDuration)
			{
				JumpForce += 500.0f;

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
	if (UEngineInput::IsPress('V'))
	{
		FSM.ChangeState(EKnightState::DEATH_DAMAGE);
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
			AddRelativeLocation(FVector{ 0.0f, Velocity * _DeltaTime, 0.0f });
		}
		if (UEngineInput::IsPress(VK_DOWN))
		{
			AddRelativeLocation(FVector{ 0.0f, -Velocity * _DeltaTime, 0.0f });
		}
	}
}

void AKnight::CreateSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSkill> SlashEffect = GetWorld()->SpawnActor<AKnightSkill>();
	SlashEffect->ChangeAnimation("SlashEffect");
	FVector Offset = FVector{ -100.0f, 0.0f};
	SlashEffect->SetLocation(this, Offset);
	bIsShowEffect = true;

	return;
}

void AKnight::CreateUpSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSkill> SlashEffect = GetWorld()->SpawnActor<AKnightSkill>();
	SlashEffect->ChangeAnimation("UpSlashEffect");
	FVector Offset = FVector{ 0.0f, 100.0f };
	SlashEffect->SetLocation(this, Offset);
	SlashEffect->EnableRotation(false); // 좌우반전에 따라 
	bIsShowEffect = true;
}

void AKnight::CreateDownSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightSkill> SlashEffect = GetWorld()->SpawnActor<AKnightSkill>();
	SlashEffect->ChangeAnimation("DownSlashEffect");
	FVector Offset = FVector{ 0.0f, -100.0f };
	SlashEffect->SetLocation(this, Offset);
	SlashEffect->EnableRotation(false);
	bIsShowEffect = true;
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

	BodyCollision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			//_Other->GetActor()->Destroy();
			// _Other->Destroy();
			//UEngineDebug::OutPutString("Enter");
		});
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
