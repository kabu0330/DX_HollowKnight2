#pragma once
#include <map>

#include <EnginePlatform/ThirdParty/FMOD/inc/fmod.hpp>

#include "math.h"
#include <mutex>

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
	// ä���� ���� ������� ����
	FMOD::Channel* Control = nullptr;
	FMOD::Sound* SoundHandle = nullptr;;
};

// ���� : FMOD ���� ���� Ŭ����
class UEngineSound 
{
public:
	ENGINEAPI UEngineSound();
	ENGINEAPI ~UEngineSound();

	ENGINEAPI static void Load(std::string_view _Path);
	ENGINEAPI static void Load(std::string_view _Name, std::string_view _Path);
	ENGINEAPI static USoundPlayer Play(std::string_view _Name);
	ENGINEAPI static UEngineSound* Find(std::string_view _Name);
	ENGINEAPI static void Release();

	ENGINEAPI static void Update();

	ENGINEAPI static void AllSoundStop();
	ENGINEAPI static void AllSoundOff();
	ENGINEAPI static void AllSoundOn();
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

