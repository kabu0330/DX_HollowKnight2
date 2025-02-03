#pragma once
#include "MonsterSkill.h"

// Ό³Έν :
class AFalseKnightHead : public AMonsterSkill
{
public:
	// constrcuter destructer
	AFalseKnightHead();
	~AFalseKnightHead();

	// delete Function
	AFalseKnightHead(const AFalseKnightHead& _Other) = delete;
	AFalseKnightHead(AFalseKnightHead&& _Other) noexcept = delete;
	AFalseKnightHead& operator=(const AFalseKnightHead& _Other) = delete;
	AFalseKnightHead& operator=(AFalseKnightHead&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	void Release() override;
};

