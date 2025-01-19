#include "PreCompile.h"
#include "StatusUnit.h"

UStatusUnit::UStatusUnit()
{
}

UStatusUnit::~UStatusUnit()
{
}

bool UStatusUnit::CreateStatus(FStatData* _Data)
{
	// 이동속도 관련
	Velocity = _Data->Velocity;
	InitVelocity = _Data->InitVelocity;
	DashSpeed = _Data->DashSpeed;

	// 체력관련
	MaxHP = _Data->MaxHP;
	HP = _Data->HP;

	// 마나 관련
	MaxMP = _Data->MaxMP;
	MP = _Data->MP;

	// 공격범위 관련
	Att = _Data->Att;
	SpellAtt = _Data->SpellAtt;

	// 피격 관련
	bIsKnockbackable = true;
	knockbackDistance = 50.0f;

	// 재화
	Geo = 0;

	return true;
}


