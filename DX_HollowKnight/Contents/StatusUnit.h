#pragma once

struct FStatusData
{
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
	int Att = 1;
	int SpellAtt = 1;

	// �ǰ� ����
	bool bIsKnockbackable = true;
	float KnockbackDistance = 50.0f;

	// ��ȭ
	int Geo = 0;
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

	bool CreateStatus(FStatusData* _Data);

	float GetVelocity()
	{
		return Velocity;
	}
	void SetVelocity(float _Velocity)
	{
		Velocity = _Velocity;
	}

	float GetInitVelocity()
	{
		return InitVelocity;
	}
	void SetInitVelocity(float _InitVelocity)
	{
		InitVelocity = _InitVelocity;
	}

	float GetDashSpeed()
	{
		return DashSpeed;
	}
	void SetDashSpeed(float _DashSpeed)
	{
		DashSpeed = _DashSpeed;
	}

	int GetMaxHP()
	{
		return MaxHP;
	}
	void SetMaxHP(int _MaxHP)
	{
		MaxHP = _MaxHP;
	}

	int GetHP()
	{
		return HP;
	}
	void SetHP(int _HP)
	{
		HP = _HP;
	}

	int GetMaxMP()
	{
		return MaxMP;
	}
	void SetMaxMP(int _MaxMP)
	{
		MaxMP = _MaxMP;
	}

	int GetMP()
	{
		return MP;
	}
	void SetMP(int _MP)
	{
		MP = _MP;
	}

	int GetAtt()
	{
		return Att;
	}
	void SetAtt(int _Att)
	{
		Att = _Att;
	}

	int GetSpellAtt()
	{
		return SpellAtt;
	}
	void SetSpellAtt(int _SpellAtt)
	{
		SpellAtt = _SpellAtt;
	}

	bool IsKnockbackable()
	{
		return bIsKnockbackable;
	}
	void SetKnockbackable(bool _bIsKnockbackable)
	{
		bIsKnockbackable = _bIsKnockbackable;
	}

	float GetKnockbackDistance()
	{
		return KnockbackDistance;
	}
	void SetKnockbackDistance(float _KnockbackDistance)
	{
		KnockbackDistance = _KnockbackDistance;
	}

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

	// ���ݷ� 
	int Att = 1;
	int SpellAtt = 1;

	// �ǰ� : �˹�
	bool bIsKnockbackable = true;
	float KnockbackDistance = 50.0f;
	
	// ��ȭ
	int Geo = 0;
};

