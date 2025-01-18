#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>
#include "ContentsRenderer.h"

class GlobalFunc
{
public:
	static void AutoScale(std::shared_ptr<UContentsRenderer> _Renderer, std::string_view _AnimationName)
	{
		UContentsRenderer::FrameAnimation* Animation = _Renderer->FindAnimation(_AnimationName);
		//Animation->IsAutoScale = true;
		//Animation->AutoScaleRatio = 1.0f;
		_Renderer->SetAutoScaleRatio(1.0f);
	}

	// 좌우반전 적용
	static void SetLocation(std::shared_ptr<class USceneComponent> _RootComponent, std::shared_ptr<AActor> _Actor, bool _Left = true, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		FVector Pos = _RootComponent->GetTransformRef().RelativeLocation;
		FVector LRPos = _OffsetPos;
		if (true == _Left)
		{
			LRPos += Pos;
			_Actor->SetActorLocation(LRPos);
			_Actor->SetActorRotation(_Rotation);
			return;
		}
		else
		{
			float Inverse = 180.0f;
			LRPos -= Pos;
			_Actor->SetActorLocation(-LRPos);
			_Actor->SetActorRotation({ _Rotation.X, _Rotation.Y + Inverse, _Rotation.Z });
			return;
		}
	}

	// 상하 모션일 때 좌우 반전
	static void SetLocationTB(std::shared_ptr<class USceneComponent> _RootComponent, std::shared_ptr<UContentsRenderer> _Renderer, bool _Left = true, const FVector& _OffsetPos = { 0.0f, 0.0f, 0.0f }, const FVector& _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		FVector Pos = _RootComponent->GetTransformRef().RelativeLocation;
		FVector LRPos = _OffsetPos;
		LRPos += Pos;
		_Renderer->SetRelativeLocation(LRPos);
		if (true == _Left)
		{
			_Renderer->SetRotation(_Rotation);
			return;
		}
		else
		{
			float Inverse = 180.0f;
			_Renderer->SetRotation({ _Rotation.X, _Rotation.Y + Inverse, _Rotation.Z });
			return;
		}
	}

	static class UEngineWinImage* LoadResource(std::string_view _Directory, std::string_view _SpriteName, UEngineWinImage* _ResImage);


};

enum class EZOrder
{
	NONE = 100,
	SKILL_FRONT = 890,
	PLAYER = 900,
	MONSTER = 910,
	SKILL_BACK = 920,
	BACKGROUND = 3900,
	PIXELCOLLISION = 3999,
	MAX
};

enum EMapObjectType
{
	BACKGROUND_COLOR,
	BACKGROUND_OBJECT,
	COLLISION_OBJECT,
	NONE_COLLISION_OBJECT,
	MAX
};

enum class EKnightState
{
	IDLE,
	RUN,
	IDLE_TO_RUN,
	RUN_TO_IDLE,
	DASH,
	JUMP,
	AIRBORN,
	LAND,
	HARD_LAND,

	LOOK_DOWN,
	LOOK_DOWN_LOOP,
	LOOK_UP,
	LOOK_UP_LOOP,

	SLASH,
	UP_SLASH,
	DOWN_SLASH,

	FOCUS,
	FOCUS_GET,
	FOCUS_END,
	FIREBALL_ANTIC,
	FIREBALL_CAST,

	DAMAGED,

	DEATH,
	DEATH_DAMAGE,
	DEATH_HEAD,
	MAX
};

enum class EMonsterState
{
	IDLE,
	WALK,
	RUN,
	TURN,

	JUMP_ANTICIPATE,
	JUMP,
	JUMP_ATTACK_ANTICIPATE,
	JUMP_ATTACK,
	LAND,
	HARD_LAND,

	DASH_ARRIVE,
	DASH_ANTICIPATE,
	DASH,
	DASH_ATTACK,
	DASH_RECOVERY,

	ATTACK_ANTICIPATE,
	ATTACK,
	ATTACK_RECOVERY,

	THROW_ANTICIPATE,
	THROW,
	THROW_RECOVERY,

	WALL_ANTICIPATE,
	WALL,
	WALL_ATTACK,
	WALL_RECOVERY,

	COUNTER_ANTICIPATE,
	COUNTER_STANCE,
	COUNTER,
	COUNTER_ATTACK,
	COUNTER_ATTACK_RECOVERY,
	COUNTER_RECOVERY,

	EVADE_ANTICIPATE,
	EVADE,

	SHIELD_ANTICIPATE,
	SHIELD_FRONT,
	SHIELD_FRONT_BUMP,
	SHIELD_TOP,
	SHIELD_TOP_BUMP,

	STUN,
	STUN_ROLL,
	STUN_ROLL_END,
	STUN_OPEN,
	STUN_HIT,

	DEATH_AIR,
	DEATH_LAND,

	FLY,
	FIRE,
	BURST,
	MAX
};
