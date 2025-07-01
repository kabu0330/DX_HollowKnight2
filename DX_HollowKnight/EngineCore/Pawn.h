#pragma once
#include <EngineCore/Actor.h>

// ���� : ���ΰ� ĳ���ʹ� ������ ��� �޾ƾ� �Ѵ�.
class APawn : public AActor
{
public:
	ENGINEAPI APawn();
	ENGINEAPI ~APawn();

protected:

private:
	// delete Function
	APawn(const APawn& _Other) = delete;
	APawn(APawn&& _Other) noexcept = delete;
	APawn& operator=(const APawn& _Other) = delete;
	APawn& operator=(APawn&& _Other) noexcept = delete;
};

