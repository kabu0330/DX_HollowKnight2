#include "PreCompile.h"
#include "MonsterManager.h"
#include "Monster.h"
#include "PlayGameMode.h"

AMonsterManager::AMonsterManager()
{
}

AMonsterManager::~AMonsterManager()
{
}

void AMonsterManager::CreateMonster(AGameMode* _GameMode)
{
	GameMode = _GameMode;
	AMonster* DashBug = GameMode->GetWorld()->SpawnActor<AMonster>().get();
	APlayGameMode* PlayGameMode = dynamic_cast<APlayGameMode*>(GameMode);
	if (nullptr == PlayGameMode)
	{
		int a = 0;
	}
	std::vector<ARoom*> Rooms = PlayGameMode->GetRoomsRef().GetRoomsRef();

	
}

