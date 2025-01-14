#include "PreCompile.h"
#include "Knight.h"
#include <EnginePlatform/EngineInput.h>
#include "KnightSkill.h"


void AKnight::SetIdle(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);
	bCanRotation = true;

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();  // ����
	ChangeDash(); // ���

	ChangeLookAnimation(); // �� �Ʒ� �Ĵٺ���

	ChangeAttackAnimation(EKnightState::IDLE); // ���� ����
	CastFocus(); // ����
	CastFireball(); // ���̾

}

void AKnight::SetRun(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;

	if (UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::RUN_TO_IDLE);
		return;
	}

	ChangeJumpAnimation();
	ChangeDash(); // ���

	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetIdleToRun(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;

	ChangeNextAnimation(EKnightState::RUN);

	ChangeJumpAnimation();
	ChangeDash(); // ���

	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetRunToIdle(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;
	bIsDashing = false;


	ChangeNextAnimation(EKnightState::IDLE);

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();
	ChangeDash(); // ���

	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetJump(float _DeltaTime)
{
	ActiveGravity();
	Jump(_DeltaTime);

	Move(_DeltaTime);


	bCanRotation = true;

	ChangeDash(); // ���

	if (true == bIsOnGround)
	{
		//FSM.ChangeState(EKnightState::LAND);
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
		ChangeAttackAnimation(EKnightState::AIRBORN);
	}
}

void AKnight::SetAirborn(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;

	ChangeDash(); // ���

	if (true == bIsOnGround)
	{
		FSM.ChangeState(EKnightState::LAND);
	}

}

void AKnight::SetLand(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	float InitJumpForce = 600.0f;
	JumpForce = InitJumpForce;

	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetHardLand(float _DeltaTime)
{

	float InitJumpForce = 600.0f;
	JumpForce = InitJumpForce;

	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetDash(float _DeltaTime)
{
	bCanRotation = false;
	bIsDashing = true;

	if (true == bIsLeft)
	{
		AddRelativeLocation(FVector{ -Velocity * _DeltaTime, 0.0f, 0.0f });
	}
	else
	{
		AddRelativeLocation(FVector{ Velocity * _DeltaTime, 0.0f, 0.0f });
	}

	//if (true == IsOnGround())
	//{
	//	ChangeNextAnimation(EKnightState::RUN_TO_IDLE);
	//	return;
	//}
	//else
	//{
	//	ChangeNextAnimation(EKnightState::AIRBORN);
	//	return;
	//}

	ChangeNextAnimation(EKnightState::RUN_TO_IDLE);

}

void AKnight::SetSlash(float _DeltaTime)
{
	Move(_DeltaTime);

	CreateSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetUpSlash(float _DeltaTime)
{
	Move(_DeltaTime);

	CreateUpSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetDownSlash(float _DeltaTime)
{
	Move(_DeltaTime);

	CreateDownSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetFocus(float _DeltaTime)
{

	//if (UEngineInput::IsFree('A'))
	//{
	//	ChangeNextAnimation(EKnightState::IDLE);
	//	return;
	//}

	ChangeNextAnimation(EKnightState::FOCUS_GET);
}

void AKnight::SetFocusGet(float _DeltaTime)
{
	if (false == bIsEffectActive)
	{
		std::shared_ptr<AKnightSkill> FocusEffect = GetWorld()->SpawnActor<AKnightSkill>();
		FocusEffect->ChangeAnimation("FocusEffect");
		//GlobalFunc::SetLocation(RootComponent, FocusEffect->GetRenderer());
		bIsEffectActive = true;
	}

	//if (UEngineInput::IsFree('A'))
	//{
	//	ChangeNextAnimation(EKnightState::IDLE);
	//	return;
	//}
	ChangeNextAnimation(EKnightState::FOCUS_END);
}

void AKnight::SetFocusEnd(float _DeltaTime)
{
	if (UEngineInput::IsPress('A'))
	{
		FSM.ChangeState(EKnightState::FOCUS);
		bIsEffectActive = false;
		return;
	}
	else // ��ų ���� ����
	{
		if (true == bIsEffectActive)
		{
			std::shared_ptr<AKnightSkill> FocusEffect = GetWorld()->SpawnActor<AKnightSkill>();
			FocusEffect->ChangeAnimation("FocusEffectEnd");
			//GlobalFunc::SetLocation(RootComponent, FocusEffect->GetRenderer());
			bIsEffectActive = false;
		}
		ChangeNextAnimation(EKnightState::IDLE);
	}
}

void AKnight::SetFireballAntic(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::FIREBALL_CAST);
}

void AKnight::SetFireballCast(float _DeltaTime)
{
	if (true == bIsOnGround)
	{
		ChangeNextAnimation(EKnightState::IDLE);
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
	}
}

void AKnight::SetLookDown(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::LOOK_DOWN_LOOP);
	if (UEngineInput::IsFree(VK_DOWN))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetLookDownLoop(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_DOWN))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetLookUp(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::LOOK_UP_LOOP);
	if (UEngineInput::IsFree(VK_UP))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetLookUpLoop(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_UP))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetDamage(float _DeltaTime)
{
	if (true == bIsOnGround)
	{
		ChangeNextAnimation(EKnightState::IDLE);
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
	}

	// �ڷ� �з�����
	// ����Ʈ ���
}

void AKnight::SetDeathDamage(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::DEATH);
}

void AKnight::SetDeath(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::DEATH_HEAD);

}

void AKnight::SetDeathHead(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetFSM()
{
	// �̵� �ִϸ��̼�
	CreateState(EKnightState::IDLE, &AKnight::SetIdle, "Idle");
	CreateState(EKnightState::RUN, &AKnight::SetRun, "Run");
	CreateState(EKnightState::RUN_TO_IDLE, &AKnight::SetRunToIdle, "RunToIdle");
	CreateState(EKnightState::IDLE_TO_RUN, &AKnight::SetIdleToRun, "IdleToRun");
	CreateState(EKnightState::DASH, &AKnight::SetDash, "Dash");
	CreateState(EKnightState::JUMP, &AKnight::SetJump, "Jump");
	CreateState(EKnightState::AIRBORN, &AKnight::SetAirborn, "Airborn");
	CreateState(EKnightState::LAND, &AKnight::SetLand, "Land");
	CreateState(EKnightState::HARD_LAND, &AKnight::SetHardLand, "HardLand");

	// ���� �ִϸ��̼�
	CreateState(EKnightState::LOOK_DOWN, &AKnight::SetLookDown, "LookDown");
	CreateState(EKnightState::LOOK_DOWN_LOOP, &AKnight::SetLookDownLoop, "LookDownLoop");
	CreateState(EKnightState::LOOK_UP, &AKnight::SetLookUp, "LookUp");
	CreateState(EKnightState::LOOK_UP_LOOP, &AKnight::SetLookUpLoop, "LookUpLoop");

	// ���� �ִϸ��̼�
	// �Ϲݰ���
	CreateState(EKnightState::SLASH, &AKnight::SetSlash, "Slash");
	CreateState(EKnightState::UP_SLASH, &AKnight::SetUpSlash, "UpSlash");
	CreateState(EKnightState::DOWN_SLASH, &AKnight::SetDownSlash, "DownSlash");

	// ����
	CreateState(EKnightState::FOCUS, &AKnight::SetFocus, "Focus");
	CreateState(EKnightState::FOCUS_GET, &AKnight::SetFocusGet, "FocusGet");
	CreateState(EKnightState::FOCUS_END, &AKnight::SetFocusEnd, "FocusEnd");
	CreateState(EKnightState::FIREBALL_ANTIC, &AKnight::SetFireballAntic, "FireballAntic");
	CreateState(EKnightState::FIREBALL_CAST, &AKnight::SetFireballCast, "FireballCast");

	// �ǰ�
	CreateState(EKnightState::DAMAGED, &AKnight::SetDamage, "Damage");

	// ���
	CreateState(EKnightState::DEATH, &AKnight::SetDeath, "Death");
	CreateState(EKnightState::DEATH_DAMAGE, &AKnight::SetDeathDamage, "DeathDamage");
	CreateState(EKnightState::DEATH_HEAD, &AKnight::SetDeathHead, "DeathHead");

	FSM.ChangeState(EKnightState::IDLE);
}

void AKnight::CreateState(EKnightState _State, StateCallback _Callback, std::string_view _AnimationName)
{
	FSM.CreateState(_State, std::bind(_Callback, this, std::placeholders::_1),
		[this, _AnimationName]()
		{
			std::string AnimationName = _AnimationName.data();
			BodyRenderer->ChangeAnimation(AnimationName);
		});
}