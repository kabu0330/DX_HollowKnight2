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
}

void UCollisionManager::LinkCollision(AGameMode* _GameMode)
{
	// �浹üũ �ؾ��Ѵ�.
	_GameMode->GetWorld()->LinkCollisionProfile("Knight", "Monster");
	_GameMode->GetWorld()->LinkCollisionProfile("Monster", "KnightSkill");
}

