#pragma once
#include <map>

#include <EnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>

#include "math.h"

class USoundPlayer 
{
public:
	friend class UEngineSound;

	ENGINEAPI USoundPlayer();
	ENGINEAPI ~USoundPlayer();

	ENGINEAPI void On();

	ENGINEAPI void Off();
	ENGINEAPI void Stop();
	ENGINEAPI bool IsPlaying();
	ENGINEAPI void SetVolume(float _Volume);
	ENGINEAPI void SwtichOnOffSound();
	ENGINEAPI void SetPosition(unsigned int _Value);
	ENGINEAPI void Loop(int Count = -1);
	ENGINEAPI void Restart();
	ENGINEAPI unsigned int GetLengthMs();

private:
	// 채널이 사운드 재생권한 소유
	FMOD::Channel* Control = nullptr;
	FMOD::Sound* SoundHandle = nullptr;;
};

// 설명 : FMOD 사운드 관리 클래스
class UEngineSound 
{
public:
	ENGINEAPI UEngineSound();
	ENGINEAPI ~UEngineSound();

	ENGINEAPI static void LoadSound(std::string_view _Path);
	ENGINEAPI static void LoadSound(std::string_view _Name, std::string_view _Path);
	ENGINEAPI static USoundPlayer Play(std::string_view _Name);
	ENGINEAPI static UEngineSound* FindSound(std::string_view _Name);
	ENGINEAPI static void Release();

	ENGINEAPI static void Update();

	ENGINEAPI static void StopAllSounds();
	ENGINEAPI static void TurnOffAllSounds();
	ENGINEAPI static void TurnOnAllSound();
	ENGINEAPI static void Init();

protected:

private:
	ENGINEAPI static std::map<std::string, UEngineSound*> Sounds;
	ENGINEAPI static std::list<USoundPlayer> Players;

	FMOD::Sound* SoundHandle;

	ENGINEAPI bool LoadResource(std::string_view _Path);

private:
	// delete Function
	UEngineSound(const UEngineSound& _Other) = delete;
	UEngineSound(UEngineSound&& _Other) noexcept = delete;
	UEngineSound& operator=(const UEngineSound& _Other) = delete;
	UEngineSound& operator=(UEngineSound&& _Other) noexcept = delete;
};

