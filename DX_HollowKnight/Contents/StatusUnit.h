#pragma once

struct FStatData
{

};

// ���� : �÷��̾�� ������ ������ �����ϰ� ����
class UStatusUnit
{
public:
	// constrcuter destructer
	UStatusUnit();
	~UStatusUnit();

	// delete Function
	UStatusUnit(const UStatusUnit& _Other) = delete;
	UStatusUnit(UStatusUnit&& _Other) noexcept = delete;
	UStatusUnit& operator=(const UStatusUnit& _Other) = delete;
	UStatusUnit& operator=(UStatusUnit&& _Other) noexcept = delete;

	bool CreateStatus(FStatData* _Data);

protected:

private:
	// �̵��ӵ� ����
	float Velocity = 100.0f;
	float InitVelocity = 0.0f;
	float DashSpeed = 0.0f;

	// ü�°���
	int MaxHP = 5;
	int HP = 5;

	// ���� ����
	int MaxMP = 99;
	int MP = 0;

	// ���ݹ��� ����
	int Attack = 1;
	int SpellAttack = 1;

	// �ǰ� ����
	bool bIsKnockbackable = true;
	float knockbackDistance = 50.0f;
	
	// ��ȭ
	int Geo = 0;
};

