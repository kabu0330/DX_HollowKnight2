#include "PreCompile.h"
#include "StatusUnit.h"

UStatusUnit::UStatusUnit()
{
}

UStatusUnit::~UStatusUnit()
{
}

bool UStatusUnit::CreateStatus(FStatusData* _Data)
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
	bIsKnockbackable = _Data->bIsKnockbackable;
	KnockbackDistance = _Data->KnockbackDistance;

	// ��ȭ
	Geo = _Data->Geo;


	if (0 > Velocity)
	{
		MSGASSERT("�̵��ӵ��� ������ ������ �� �����ϴ�. �ּ� 0 �̻����� �������ּ���.");
		return false;
	}
	if (1 > HP)
	{
		MSGASSERT("ü���� �ּ� 1 �̻����� �������ּ���.");
		return false;
	}
	if (HP > MaxHP)
	{
		MSGASSERT("�ִ� ü�º��� HP�� �� ���� ������ �� �����ϴ�.");
		return false;
	}
	if (0 > MP)
	{
		MSGASSERT("������ ������ ������ �� �����ϴ�. �ּ� 0 �̻����� �������ּ���.");
		return false;
	}
	if (MP > MaxMP)
	{
		MSGASSERT("�ִ� �������� MP�� �� ���� ������ �� �����ϴ�.");
		return false;
	}
	if (1 > Att)
	{
		MSGASSERT("���ݷ��� �ּ� 1 �̻����� �������ּ���.");
		return false;
	}
	if (0 > SpellAtt)
	{
		MSGASSERT("���� ���ݷ��� ������ ������ �� �����ϴ�. �ּ� 0 �̻����� �������ּ���.");
		return false;
	}

	return true;
}


