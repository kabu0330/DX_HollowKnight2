#pragma once

// 설명 : 상수값을 정의
namespace RoomPos
{
	// 흙의 마을에서 잊혀진 교차로로
	FVector FromDirtmouth = { 9400, -3408 }; // 이미지 좌표
	FVector ToCrossroads1 = { 9384, -5595 }; // 월드 좌표

	// 잊혀진 교차로에서 다시 마을로
	FVector FromCrossroads1 = { 9384, -5595 };
	FVector ToDirtmouth = { 9262, -3108 };

	FVector Crossroads1Right = { 5800, -1900 };
	FVector Crossroads2Left = { 12137, -5443 };


	FVector ToCrossroads1Right = { 12137, -5443 };
}

