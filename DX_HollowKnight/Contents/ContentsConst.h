#pragma once

// ���� : ������� ����
class RoomPos
{
public:
	// ���� �������� ������ �����η�
	inline static FVector FromDirtmouth = { 9300, -3408 }; // ������ǥ
	inline static FVector ToCrossroads1 = { 9284, -5495 }; 

	// ������ �����ο��� �ٽ� ������
	inline static FVector FromCrossroads1 = { 9384, -5595 };
	inline static FVector ToDirtmouth = { 9000, -3000 };

	// ������ 1
	inline static FVector Crossroads1Left = { 6210, -5740 };
	inline static FVector Crossroads3Right = { 5450, -5760 };

	inline static FVector Crossroads1Right = { 11940, -5445 };
	inline static FVector Crossroads2Left = { 12650, -5380 };

	// ������ 2


	// ������ 3


};

class InitPos
{
public:
	inline static FVector Dirtmouth_well = { 9000, -3000 };
	inline static FVector CrossroadsEntrance = { 9390, -5500 };
};
