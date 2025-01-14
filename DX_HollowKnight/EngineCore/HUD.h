#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AHUD : public AActor
{
public:
	// constrcuter destructer
	ENGINEAPI AHUD();
	ENGINEAPI ~AHUD();

	// delete Function
	AHUD(const AHUD& _Other) = delete;
	AHUD(AHUD&& _Other) noexcept = delete;
	AHUD& operator=(const AHUD& _Other) = delete;
	AHUD& operator=(AHUD&& _Other) noexcept = delete;

	void CreateWidget();

protected:

private:

};

