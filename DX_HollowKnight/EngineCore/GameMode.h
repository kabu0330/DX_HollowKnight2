#pragma once
#include "Actor.h"

// Ό³Έν :
class AGameMode : public AActor
{
public:
	ENGINEAPI AGameMode();
	ENGINEAPI ~AGameMode();

protected:

private:
	// delete Function
	AGameMode(const AGameMode& _Other) = delete;
	AGameMode(AGameMode&& _Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& _Other) = delete;
	AGameMode& operator=(AGameMode&& _Other) noexcept = delete;
};

