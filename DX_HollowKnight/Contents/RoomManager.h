#pragma once
#include "Room.h"

// ���� :
class URoomManager
{
public:
	// constrcuter destructer
	URoomManager();
	~URoomManager();

	// delete Function
	URoomManager(const URoomManager& _Other) = delete;
	URoomManager(URoomManager&& _Other) noexcept = delete;
	URoomManager& operator=(const URoomManager& _Other) = delete;
	URoomManager& operator=(URoomManager&& _Other) noexcept = delete;

	void CreateAndLinkRoom(class AGameMode* _GameMode);
	

protected:

private:
	inline static class AGameMode* GameMode = nullptr;
	std::shared_ptr<ARoom> CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio = 1.0f);
	void SetInitCurRoom(ARoom* _InitRoom);
	void LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size, float _ScaleRatio = 1.0f);


	std::vector<std::shared_ptr<ARoom>> Rooms;
};

