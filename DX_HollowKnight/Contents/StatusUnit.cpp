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
	// �̵��ӵ� ����
	Velocity = _Data->Velocity;
	InitVelocity = _Data->InitVelocity;
	DashSpeed = _Data->DashSpeed;

	// ü�°���
	MaxHP = _Data->MaxHP;
	HP = _Data->HP;

	// ���� ����
	MaxMP = _Data->MaxMP;
	MP = _Data->MP;

	// ���ݹ��� ����
	Att = _Data->Att;
	SpellAtt = _Data->SpellAtt;

	// �ǰ� ����
	bIsKnockbackable = true;
	knockbackDistance = 50.0f;

	// ��ȭ
	Geo = 0;

	return true;
}


