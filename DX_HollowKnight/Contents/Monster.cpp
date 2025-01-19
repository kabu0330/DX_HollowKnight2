#include "PreCompile.h"
#include "Monster.h"

AMonster::AMonster()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;


	Offset = { 0.0f, -100.0f };
	ZSort = static_cast<float>(EZOrder::MONSTER);

	// Renderer
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);

	// Collision
	BodyCollision = CreateDefaultSubObject<UCollision>();
	BodyCollision->SetupAttachment(RootComponent);
}

void AMonster::BeginPlay()
{
	AActor::BeginPlay();
	CreateAnimation();
	CreateCollision();
	SetCollisionEvent();
	SetFSM();
}

void AMonster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CheckCurRoom(); // 현재 나이트가 속한 룸 위치를 계속 체크
	SetPause(); // 나이트가 몬스터가 속한 룸과 일치하지 않으면 bIsPause로 정지
	ActivePixelCollision();

	FSM.Update(_DeltaTime);

}

void AMonster::SetCollisionEvent()
{
	BodyCollision->SetCollisionEnter([](UCollision* _This, UCollision* _Other)
		{
			UEngineDebug::OutPutString("Monster Collision Event Enter");
		});
}

void AMonster::Move(float _DeltaTime)
{
}

void AMonster::TimeElapsed(float _DeltaTime)
{
}

void AMonster::DebugInput(float _DeltaTime)
{
}

void AMonster::CreateAnimation()
{
	std::string HuskBully = "HuskBully";
	float FrameTime = 0.2f;
	BodyRenderer->CreateAnimation("Idle", HuskBully, 0, 5, FrameTime);
	BodyRenderer->CreateAnimation("Walk", HuskBully, 6, 12, FrameTime);
	BodyRenderer->CreateAnimation("Turn", HuskBully, 13, 14, FrameTime);
	BodyRenderer->CreateAnimation("Attack_Anticipate", HuskBully, 15, 18, FrameTime);
	BodyRenderer->CreateAnimation("Attack_Lunge", HuskBully, 19, 20, FrameTime);
	BodyRenderer->CreateAnimation("Attack_Cooldown", HuskBully, 21, 21, FrameTime);
	BodyRenderer->CreateAnimation("Death_Air", HuskBully, 22, 22, FrameTime);
	BodyRenderer->CreateAnimation("Death_Land", HuskBully, 23, 30, FrameTime);



	BodyRenderer->ChangeAnimation("Idle");
	BodyRenderer->SetWorldLocation({ Offset.X, Offset.Y, ZSort });

	std::string Vengefly = "Vengefly";
	BodyRenderer->CreateAnimation(Vengefly, Vengefly, 0, 2, 0.2f);

}

void AMonster::CreateCollision()
{
	BodyCollision->SetScale3D(BodyRenderer->GetScale() / 2.0f);
	BodyCollision->SetWorldLocation({ Offset.X, Offset.Y / 2.0f, ZSort });
	//BodyCollision->SetActive(false);
	BodyCollision->SetCollisionProfileName("Monster");
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
	if (bIsLeft = true)
	{
		SetActorRelativeScale3D({ 1.0f, 1.0f, 1.0f });
	}
	if (bIsLeft = false)
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

void AMonster::ActivePixelCollision()
{
	if (true == IsCurRoom())
	{
		ParentRoom->CheckPixelCollisionWithGravity(this, BodyRenderer.get());
		ParentRoom->CheckPixelCollisionWithWall(this, BodyRenderer.get(), Velocity, bIsLeft);
		ParentRoom->CheckPixelCollisionWithCeil(this, BodyRenderer.get(), Velocity, bIsLeft);
	}
}

