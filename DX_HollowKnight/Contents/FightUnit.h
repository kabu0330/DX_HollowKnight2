#pragma once

// ���� : �������� ��ȣ�ۿ� �Լ� ����
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

