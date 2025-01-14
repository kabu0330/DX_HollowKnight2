#include "PreCompile.h"
#include "CollisionManager.h"

UCollisionManager::UCollisionManager()
{
}

UCollisionManager::~UCollisionManager()
{
}

void UCollisionManager::CreateCollisionProfile(AGameMode* _GameMode)
{
	_GameMode->GetWorld()->CreateCollisionProfile("Knight");
	_GameMode->GetWorld()->CreateCollisionProfile("KnightSkill");
	_GameMode->GetWorld()->CreateCollisionProfile("Monster");
	_GameMode->GetWorld()->CreateCollisionProfile("Object");
	_GameMode->GetWorld()->CreateCollisionProfile("Door");
}

void UCollisionManager::LinkCollision(AGameMode* _GameMode)
{
	// 충돌체크 해야한다.
	_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Monster");
	_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Door");
	_GameMode->GetWorld()->LinkCollisionProfile("Door", "Knight");
	_GameMode->GetWorld()->LinkCollisionProfile("Monster", "KnightSkill");
}

