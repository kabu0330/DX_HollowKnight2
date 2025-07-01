#include "PreCompile.h"
#include "EngineSound.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>
#include <mutex>

std::map<std::string, UEngineSound*> UEngineSound::Sounds;
std::list<USoundPlayer> UEngineSound::Players;

// 사운드를 제어하기위한 핸들
FMOD::System* SoundSystem = nullptr;
std::mutex SoundCriticalSection;

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#endif

USoundPlayer::USoundPlayer()
{
}
USoundPlayer::~USoundPlayer()
{
}

void USoundPlayer::On()
{
	Control->setPaused(false);
}

void USoundPlayer::Off()
{
	Control->setPaused(true);
}

void USoundPlayer::Stop()
{
	Control->stop();
	Control = nullptr;
}

bool USoundPlayer::IsPlaying()
{
	bool Check = true;
	Control->isPlaying(&Check);
	return Check;
}

void USoundPlayer::SetVolume(float _Volume)
{
	_Volume = UEngineMath::Clamp(_Volume, 0.0f, 1.0f);

	Control->setVolume(_Volume);
}

void USoundPlayer::SwtichOnOffSound()
{
	bool Check = false;
	Control->getPaused(&Check);

	if (true == Check)
	{
		Control->setPaused(false);
	}
	else
	{
		Control->setPaused(true);
	}
}

void USoundPlayer::SetPosition(unsigned int _Value)
{
	Control->setPosition(_Value, FMOD_TIMEUNIT_MS);
}

void USoundPlayer::Loop(int Count/* = -1*/)
{
	Control->setLoopCount(Count);
}

void USoundPlayer::Restart()
{
	SetPosition(0);
}

unsigned int USoundPlayer::GetLengthMs()
{
	unsigned int ResultLength = 0;
	SoundHandle->getLength(&ResultLength, FMOD_TIMEUNIT_MS);
	return ResultLength;
}

void UEngineSound::Init()
{
	if (FMOD_RESULT::FMOD_OK != FMOD::System_Create(&SoundSystem))
	{
		MSGASSERT("FMOD 시스템 초기화에 실패했습니다.");
		return;
	}

	if (FMOD_RESULT::FMOD_OK != SoundSystem->init(32, FMOD_DEFAULT, nullptr))
	{
		MSGASSERT("FMOD 시스템 초기화에 실패했습니다.");
		return;
	}
}

void UEngineSound::StopAllSounds()
{
	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		USoundPlayer& CurSoundPlayer = *StartIter;
		CurSoundPlayer.Stop();
	}
}

void UEngineSound::TurnOnAllSound()
{
	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		USoundPlayer& CurSoundPlayer = *StartIter;
		CurSoundPlayer.On();
	}
}

void UEngineSound::TurnOffAllSounds()
{
	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		USoundPlayer& CurSoundPlayer = *StartIter;
		CurSoundPlayer.Off();
	}
}

void UEngineSound::Update()
{
	if (nullptr == SoundSystem)
	{
		return;
	}

	std::list<USoundPlayer>::iterator StartIter = Players.begin();
	std::list<USoundPlayer>::iterator EndIter = Players.end();

	for (; StartIter != EndIter; )
	{
		USoundPlayer& CurSoundPlayer = *StartIter;

		if (true == CurSoundPlayer.IsPlaying())
		{
			++StartIter;
			continue;
		}

		StartIter = Players.erase(StartIter);
		
	}

	if (FMOD_RESULT::FMOD_OK != SoundSystem->update())
	{
		MSGASSERT("FMOD 시스템 업데이트에 실패했습니다.");
	}
}

void UEngineSound::Release()
{
	std::map<std::string, UEngineSound*>::iterator StartIter = Sounds.begin();
	std::map<std::string, UEngineSound*>::iterator EndIter = Sounds.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (nullptr != StartIter->second)
		{
			delete StartIter->second;
			StartIter->second = nullptr;
		}
	}

	Sounds.clear();

	if (nullptr != SoundSystem)
	{
		SoundSystem->release();
		SoundSystem = nullptr;
	}
}

UEngineSound::UEngineSound()
{
}

UEngineSound::~UEngineSound()
{
	
	
	if (nullptr != SoundHandle)
	{
		SoundHandle->release();
		SoundHandle = nullptr;
	}
}

void UEngineSound::LoadSound(std::string_view _Path)
{
	UEnginePath EnginePath = UEnginePath(_Path);
	std::string FileName = EnginePath.GetFileNameToString();

	UEngineSound::LoadSound(FileName.data(), _Path);
}

void UEngineSound::LoadSound(std::string_view _Name, std::string_view _Path)
{
	{
		std::lock_guard<std::mutex> Lock(SoundCriticalSection);

		std::string UpperString = UEngineString::ToUpper(_Name);

		UEngineSound* NewSound = new UEngineSound();

		if (false != UEngineSound::Sounds.contains(UpperString))
		{
			delete NewSound;
			MSGASSERT("이미 로드한 사운드입니다. " + UpperString);
			return;
		}

		if (false == NewSound->LoadResource(_Path))
		{
			delete NewSound;
			MSGASSERT("사운드 로드에 실패했습니다. 경로를 확인해주세요. " + UpperString);
			return;
		}

		UEngineSound::Sounds.insert({ UpperString, NewSound });
	}
}

UEngineSound* UEngineSound::FindSound(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == Sounds.contains(UpperString))
	{
		return nullptr;
	}

	return Sounds[UpperString];
}

USoundPlayer UEngineSound::Play(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	UEngineSound* Sound = FindSound(_Name);

	if (nullptr == Sound)
	{
		MSGASSERT("사운드 목록에 대상이 없습니다. 사운드를 추가해 주세요. " + UpperString);
	}

	FMOD::Channel* Ch = nullptr;

	SoundSystem->playSound(Sound->SoundHandle, nullptr, false, &Ch);

	Ch->setLoopCount(0);
	Ch->setVolume(1.0f);

	USoundPlayer NewPlayer;
	NewPlayer.Control = Ch;
	NewPlayer.SoundHandle = Sound->SoundHandle;

	Players.push_back(NewPlayer);

	return NewPlayer;
}


bool UEngineSound::LoadResource(std::string_view _Path)
{
	SoundSystem->createSound(_Path.data(), FMOD_LOOP_NORMAL, nullptr, &SoundHandle);

	if (nullptr == SoundHandle)
	{
		MSGASSERT("사운드 로딩에 실패했습니다. 경로를 확인해 주세요." + std::string(_Path));
		return false;
	}

	return true;
}