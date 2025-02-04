#include "PreCompile.h"
#include "Knight.h"
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"
#include "KnightSkill.h"
#include "Effect.h"

void AKnight::SetIdle(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);
	RecoveryIdle(); // ���� ���� ���·� ��ȯ

	Stat.SetBeingHit(false);

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();  // ����
	ChangeDash(); // ���

	ChangeLookAnimation(); // �� �Ʒ� �Ĵٺ���

	CastFocus(); // ����
	CastFireball(); // ���̾

	ChangeAttackAnimation(EKnightState::IDLE); // ���� ����
}

void AKnight::SetRun(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;
	Stat.SetBeingHit(false);

	if (UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::RUN_TO_IDLE);
		return;
	}

	ChangeJumpAnimation();
	ChangeDash(); // ���

	CastFireball();

	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetIdleToRun(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;
	Stat.SetBeingHit(false);

	ChangeJumpAnimation();
	ChangeDash(); // ���

	CastFireball(); 

	ChangeNextState(EKnightState::RUN);
	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetRunToIdle(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true;
	bIsDashing = false; // �� �������� ���ƿ;߸� ��� ���°� ������ ������ �Ǵ�
	Stat.SetBeingHit(false);

	ChangeNextState(EKnightState::IDLE);

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();
	ChangeDash(); // ���

	CastFireball();

	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetJump(float _DeltaTime)
{
	ActiveGravity();
	Jump(_DeltaTime);

	Move(_DeltaTime);

	bCanRotation = true;

	ChangeDash(); // ���
	CastFireball();

	ChangeAttackAnimation(EKnightState::AIRBORN); // ���� ����


	if (true == bIsOnGround)
	{
		ChangeNextState(EKnightState::LAND);
	}
	else
	{
		ChangeNextState(EKnightState::AIRBORN);
		ChangeAttackAnimation(EKnightState::AIRBORN);
	}
}

void AKnight::SetAirborn(float _DeltaTime)
{
 	ActiveGravity();
	Move(_DeltaTime);

	RecoveryIdle(); // ���� ���� ���·� ��ȯ
	bIsDashing = false;

	ChangeDash(); // ���
	CastFireball();

	ChangeAttackAnimation(EKnightState::AIRBORN); // ���� ����

	if (true == bIsOnGround)
	{
		FSM.ChangeState(EKnightState::LAND);
	}
}

void AKnight::SetLand(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);
	Stat.SetBeingHit(false);

	float InitJumpForce = 600.0f;
	JumpForce = InitJumpForce;

	ChangeAttackAnimation(EKnightState::IDLE); // ���� ����

	ChangeNextState(EKnightState::IDLE);
}

void AKnight::SetHardLand(float _DeltaTime)
{
	ActiveGravity();
	float InitJumpForce = 600.0f;
	JumpForce = InitJumpForce;

	ChangeNextState(EKnightState::IDLE);
}

void AKnight::SetDash(float _DeltaTime)
{
	Dash();

	if (true == IsOnGround())
	{
		ChangeNextState(EKnightState::RUN_TO_IDLE);
		return;
	}
	else
	{
		ChangeNextState(EKnightState::AIRBORN);
		return;
	}

	//ChangeNextAnimation(EKnightState::RUN_TO_IDLE);

}

void AKnight::SetSlash(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	bCanRotation = true; // ȸ�� ���
	bIsShowEffect;

	CreateSlashEffect();
	ChangePrevState();
}

void AKnight::SetUpSlash(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	CreateUpSlashEffect();
	ChangePrevState();
}

void AKnight::SetDownSlash(float _DeltaTime)
{
	ActiveGravity();
	Move(_DeltaTime);

	CreateDownSlashEffect();
	ChangePrevState();
}

void AKnight::SetFocus(float _DeltaTime)
{
	ActiveGravity();
	bIsFocusEffect = false;
	bIsFocusEndEffect = false;

	if (UEngineInput::IsUp('A') || 22 > Stat.GetMp())
	{
		ChangeNextState(EKnightState::IDLE);
		return;
	}
	else
	{
		ChangeNextState(EKnightState::FOCUS_GET);
		return;
	}
}

void AKnight::SetFocusGet(float _DeltaTime)
{
	ActiveGravity();
	CreateFocusEffect();

	if (UEngineInput::IsUp('A'))
	{
		ChangeNextState(EKnightState::IDLE);
		return;
	}

	ChangeNextState(EKnightState::FOCUS_END);
}

void AKnight::SetFocusEnd(float _DeltaTime)
{
	ActiveGravity();
	CreateFocusEndEffect();
	bIsFocusEndEffect = true;

	if (UEngineInput::IsPress('A'))
	{
		FSM.ChangeState(EKnightState::FOCUS);
		bIsFocusEffect = false;
		return;
	}
	else // ��ų ���� ����
	{
		ChangeNextState(EKnightState::IDLE);
	}
}

void AKnight::SetFireballAntic(float _DeltaTime)
{
	bCanRotation = false;
	ChangeNextState(EKnightState::FIREBALL_CAST);
}

void AKnight::SetFireballCast(float _DeltaTime)
{
	bCanRotation = false;

	CreateFireballEffect(); // ����Ʈ �߻�

	if (true == bIsOnGround)
	{
		ChangeNextState(EKnightState::IDLE);
	}
	else
	{
		ChangeNextState(EKnightState::AIRBORN);
	}
}

void AKnight::SetLookDown(float _DeltaTime)
{
	ChangeNextState(EKnightState::LOOK_DOWN_LOOP);
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
	ChangeNextState(EKnightState::LOOK_UP_LOOP);
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

void AKnight::SetStun(float _DeltaTime)
{
	// �ڷ� �з�����

	// ����Ʈ ���
	CreateStunEffect();
	APlayGameMode::GetPauseRef() = true;

	TimeEventer->AddEndEvent(0.3f, [this]()
		{
			APlayGameMode::GetPauseRef() = false;
			BodyRenderer->ColorData.PlusColor = { 0.0f, 0.0f, 0.0f, 0.0f };
		});

	if (true == bIsOnGround)
	{
		ChangeNextState(EKnightState::IDLE);
	}
	else
	{
		ChangeNextState(EKnightState::AIRBORN);
	}
}

void AKnight::SetDeathDamage(float _DeltaTime)
{
	BodyCollision->SetActive(false);
	ChangeNextState(EKnightState::DEATH);
}

void AKnight::SetDeath(float _DeltaTime)
{
	ChangeNextState(EKnightState::DEATH_HEAD);
}

void AKnight::SetDeathHead(float _DeltaTime)
{
	ActiveGravity();

	AddActorRotation({ 0.0f, 0.0f, -5.0f * _DeltaTime });
	BodyRenderer->SetRelativeLocation({ 0.0f, -20.0f, 0.0f });
	if (false == bCanReset)
	{
		bCanReset = true;
		TimeEventer->AddEndEvent(2.0f, std::bind(&AKnight::ResetLevel, this));
	}
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
	//CreateState(EKnightState::SLASH, &AKnight::SetSlash, "Slash");
	CreateState(EKnightState::SLASH, &AKnight::SetSlash, "SlashAlt");
	CreateState(EKnightState::UP_SLASH, &AKnight::SetUpSlash, "UpSlash");
	CreateState(EKnightState::DOWN_SLASH, &AKnight::SetDownSlash, "DownSlash");

	// ����
	CreateState(EKnightState::FOCUS, &AKnight::SetFocus, "Focus");
	CreateState(EKnightState::FOCUS_GET, &AKnight::SetFocusGet, "FocusGet");
	CreateState(EKnightState::FOCUS_END, &AKnight::SetFocusEnd, "FocusEnd");
	CreateState(EKnightState::FIREBALL_ANTIC, &AKnight::SetFireballAntic, "FireballAntic");
	CreateState(EKnightState::FIREBALL_CAST, &AKnight::SetFireballCast, "FireballCast");

	// �ǰ�
	CreateState(EKnightState::STUN, &AKnight::SetStun, "Stun");

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