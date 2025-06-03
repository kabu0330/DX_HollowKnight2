#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class UCollisionManager
{
public:
	UCollisionManager();
	virtual ~UCollisionManager() = 0;

	static void CreateCollisionProfile(AGameMode* _GameMode);
	static void LinkCollision(AGameMode* _GameMode);
protected:

private:
	// delete Function
	UCollisionManager(const UCollisionManager& _Other) = delete;
	UCollisionManager(UCollisionManager&& _Other) noexcept = delete;
	UCollisionManager& operator=(const UCollisionManager& _Other) = delete;
	UCollisionManager& operator=(UCollisionManager&& _Other) noexcept = delete;
};

