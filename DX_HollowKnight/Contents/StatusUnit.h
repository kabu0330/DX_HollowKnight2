#pragma once

struct FStatData
{

};

// 설명 : 플레이어와 몬스터의 스텟을 설정하고 관리
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
	// 이동속도 관련
	float Velocity = 100.0f;
	float InitVelocity = 0.0f;
	float DashSpeed = 0.0f;

	// 체력관련
	int MaxHP = 5;
	int HP = 5;

	// 마나 관련
	int MaxMP = 99;
	int MP = 0;

	// 공격범위 관련
	int Attack = 1;
	int SpellAttack = 1;

	// 피격 관련
	bool bIsKnockbackable = true;
	float knockbackDistance = 50.0f;
	
	// 재화
	int Geo = 0;
};

