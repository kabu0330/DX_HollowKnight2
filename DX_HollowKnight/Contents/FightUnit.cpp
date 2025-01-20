#include "PreCompile.h"
#include "FightUnit.h"


UFightUnit::UFightUnit()
{
}

UFightUnit::~UFightUnit()
{
}

void UFightUnit::OnHit(AKnight* _Knight, int _Att)
{
	UStatusUnit& Stat = _Knight->GetStatRef();
	Stat.AddHp(-_Att); // 체력 감소

	// 스턴 상태
	Stat.SetStun(true);
	Stat.SetIsBeingHit(true);
	_Knight->GetFSMRef().ChangeState(EKnightState::STUN);
}

void UFightUnit::OnHit(AMonster* _Monster, int _Att)
{
	UStatusUnit& Stat = _Monster->GetStatRef();
	Stat.AddHp(-_Att);

	Stat.SetStun(true);
	Stat.SetIsBeingHit(true);
	//_Monster->GetFSMRef().ChangeState(EMonsterState::HIT);
}

