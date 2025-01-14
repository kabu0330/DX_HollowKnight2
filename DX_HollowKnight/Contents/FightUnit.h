#pragma once

// 설명 : 전투관련 상호작용 함수 집합
class UFightUnit
{
public:
	// constrcuter destructer
	UFightUnit();
	~UFightUnit();

	// delete Function
	UFightUnit(const UFightUnit& _Other) = delete;
	UFightUnit(UFightUnit&& _Other) noexcept = delete;
	UFightUnit& operator=(const UFightUnit& _Other) = delete;
	UFightUnit& operator=(UFightUnit&& _Other) noexcept = delete;

protected:

private:

};

