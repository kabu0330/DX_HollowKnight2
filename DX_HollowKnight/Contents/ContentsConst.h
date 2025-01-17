#pragma once

// ���� : ������� ����
class RoomPos
{
public:
	// ���� �������� ������ �����η�
	inline static FVector FromDirtmouth = { 9400, -3408 }; // �̹��� ��ǥ
	inline static FVector ToCrossroads1 = { 9384, -5595 }; // ���� ��ǥ

	// ������ �����ο��� �ٽ� ������
	inline static FVector FromCrossroads1 = { 9384, -5595 };
	inline static FVector ToDirtmouth = { 9075, -3060 };

	inline static FVector Crossroads1Right = { 5800, -1900 };
	inline static FVector Crossroads2Left = { 12137, -5443 };

	inline static FVector ToCrossroads1Right = { 12137, -5443 };
};

class InitPos
{
public:
	inline static FVector Dirtmouth_well = { 9262, -3108 };
};
