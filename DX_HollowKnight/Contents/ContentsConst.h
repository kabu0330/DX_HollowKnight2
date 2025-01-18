#pragma once

// 설명 : 상수값을 정의
class RoomPos
{
public:
	// 흙의 마을에서 잊혀진 교차로로
	inline static FVector FromDirtmouth = { 9300, -3408 }; // 월드좌표
	inline static FVector ToCrossroads1 = { 9284, -5495 }; 

	// 잊혀진 교차로에서 다시 마을로
	inline static FVector FromCrossroads1 = { 9384, -5595 };
	inline static FVector ToDirtmouth = { 9000, -3000 };

	// 교차로 1
	inline static FVector Crossroads1Left = { 6210, -5740 };
	inline static FVector Crossroads3Right = { 5450, -5760 };

	inline static FVector Crossroads1Right = { 11940, -5445 };
	inline static FVector Crossroads2Left = { 12650, -5380 };

	// 교차로 2


	// 교차로 3


};

class InitPos
{
public:
	inline static FVector Dirtmouth_well = { 9000, -3000 };
	inline static FVector CrossroadsEntrance = { 9390, -5500 };
};
