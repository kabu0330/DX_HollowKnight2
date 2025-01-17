#pragma once

// 설명 : 상수값을 정의
class RoomPos
{
public:
	// 흙의 마을에서 잊혀진 교차로로
	inline static FVector FromDirtmouth = { 9400, -3408 }; // 이미지 좌표
	inline static FVector ToCrossroads1 = { 9384, -5595 }; // 월드 좌표

	// 잊혀진 교차로에서 다시 마을로
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
