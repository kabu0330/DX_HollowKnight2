#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineSound.h>
#include <EnginePlatform/EngineInput.h>
#include "IContentCore.h"
#include "EngineResources.h"
#include "EngineConstantBuffer.h"
#include "EngineGUI.h"
#include "Level.h"
#include "GameInstance.h"

UEngineCore* GEngine = nullptr;

UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return GEngine->Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return GEngine->MainWindow;
}

float UEngineCore::GetDeltaTime()
{
	float Time = GEngine->Timer.GetDeltaTime();

	return Time;
}

double UEngineCore::GetDoubleDeltaTime()
{
	double Time = GEngine->Timer.GetDoubleDeltaTime();

	return Time;
}

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::GetAllLevelMap()
{
	return GEngine->LevelMap;
}

UGameInstance* UEngineCore::GetGameInstance()
{
	return GEngine->GameInstance.get();
}

UEngineWorkThreadPool& UEngineCore::GetThreadPool()
{
	return GEngine->ThreadPool;
}

FVector UEngineCore::GetScreenScale()
{
	return GEngine->Data.WindowSize;
}

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::CreateAndShowWindow(HINSTANCE _Instance)
{
	UEngineWindow::InitEngineWindow(_Instance);
	GEngine->MainWindow.Open("MainWindow");
}

void UEngineCore::LoadContents(std::string_view _DllName)
{
	UEngineDirectory Dir;

	Dir.MoveParentToDirectory("Build");
	Dir.MovePath("bin/x64");

#ifdef _DEBUG
	Dir.MovePath("Debug");
#else
	Dir.MovePath("Release");
#endif

	UEngineFile File = Dir.GetFile(_DllName);
	std::string FullPath = File.GetPathToString();

	GEngine->ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == GEngine->ContentsDLL)
	{
		MSGASSERT("Contents dll 파일을 로드할 수 없습니다.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentCore>&) = 
		(INT_PTR(__stdcall*)(std::shared_ptr<IContentCore>&))GetProcAddress(GEngine->ContentsDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("컨텐츠 코어 내부에 CreateContentsCoreDefine을 정의하지 않았습니다.");
		return;
	}

	Ptr(GEngine->Core);

	if (nullptr == GEngine->Core)
	{
		MSGASSERT("컨텐츠 코어 생성에 실패했습니다.");
		return;
	}
}

void UEngineCore::StartEngine(HINSTANCE _Instance, std::string_view _DllName)
{
	UEngineDebug::CheckMemoryLeak();

	UEngineCore EngineCore;

	GEngine = &EngineCore;

	GEngine->ThreadPool.Initialize();

	CreateAndShowWindow(_Instance);

	LoadContents(_DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
#ifdef _DEBUG
			UEngineDebug::StartConsole();
#endif

			UEngineSound::Init();

			GEngine->Device.SetupRenderingPipeline();

			GEngine->Core->StartEngine(GEngine->Data);
			
			GEngine->MainWindow.SetWindowPosAndScale(GEngine->Data.WindowPos, GEngine->Data.WindowSize);

			GEngine->Device.CreateBackBuffer(GEngine->MainWindow);

			UEngineGUI::Init();
		},
		[]()
		{
			EngineFrame();
		},
		[]()
		{
			EndEngine();
		});	
}

// 헤더 순환 참조를 막기 위한 함수분리
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	if (true == GEngine->LevelMap.contains(_Name.data()))
	{
		MSGASSERT("같은 이름의 레벨을 또 만들수는 없습니다." + std::string(_Name.data()));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(_Name);

	GEngine->LevelMap.insert({ _Name.data(), Ptr}); // 생성된 레벨은 모두 LevelMap에 저장

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == GEngine->LevelMap.contains(UpperString))
	{
		MSGASSERT(std::string(_Name) + " 은 생성되지 않은 레벨입니다. \n CreateLevel 함수를 사용해 레벨을 생성 후 OpenLevel 함수를 사용해야 합니다.");
		return;
	}

	GEngine->NextLevel = GEngine->LevelMap[UpperString];
}

void UEngineCore::EngineFrame()
{
	// 현재 레벨을 리셋할거야?
	if (true == GEngine->IsCurLevelReset)
	{
		GEngine->CurLevel = nullptr;
		GEngine->IsCurLevelReset = false;
	}

	if (nullptr != GEngine->NextLevel) // 레벨체인지할 Level이 존재하면
	{
		if (nullptr != GEngine->CurLevel) // 현재 레벨이 종료되면서 할 일이 있으면 마무리 하고
		{
			GEngine->CurLevel->EndLevel();
		}

		GEngine->CurLevel = GEngine->NextLevel; // 레벨을 바꾼다.

		GEngine->CurLevel->StartLevel(); // 새로운 레벨에서 처음 세팅할 작업을 먼저 진행하고
		GEngine->NextLevel = nullptr; // NextLevel 포인터의 역할은 다했다.
		GEngine->Timer.TimeStart(); // 델타 타임도 처음부터 다시 갱신한다. 혹시 모를 오류가 있을까봐
	}

	GEngine->Timer.TimeCheck(); // 델타 타임 체크
	float DeltaTime = GEngine->Timer.GetDeltaTime();

	if (true == GEngine->MainWindow.IsFocus()) // 윈도우가 포커스되었을 때만
	{
		UEngineInput::CheckInput(DeltaTime); // 키 입력 체크
	}
	else {
		UEngineInput::ResetKey();
	}

	UEngineSound::Update();

	// Core에서 Level이 관리하는 Actor, Renderer, Collision를 'Windows메시지루프'에서 돌려준다.
	GEngine->CurLevel->Tick(DeltaTime);
	GEngine->CurLevel->Render(DeltaTime);
	// GUI랜더링은 기존 랜더링이 다 끝나고 해주는게 좋다.

	GEngine->CurLevel->Collision(DeltaTime);
	GEngine->CurLevel->Release(DeltaTime);
}

void UEngineCore::EndEngine()
{
	UEngineGUI::Release();

	GEngine->Device.Release();

	UEngineResources::Release();
	UEngineConstantBuffer::Release();
	UEngineSound::Release();

	GEngine->CurLevel = nullptr;
	GEngine->NextLevel = nullptr;
	GEngine->LevelMap.clear();

	UEngineDebug::EndConsole();
}

void UEngineCore::SetGameInstance(std::shared_ptr<UGameInstance> _Inst)
{
	GEngine->GameInstance = _Inst;
}

bool UEngineCore::IsCurLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	if (GEngine->CurLevel->GetName() != UpperName) 
	{
		DestroyLevel(_LevelName); // 지우고
		return false;
	}
	return true;
}

std::shared_ptr<ULevel> UEngineCore::ReadyToNextLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	std::map<std::string, std::shared_ptr<ULevel>>::iterator FindIter = GEngine->LevelMap.find(UpperName);
	GEngine->LevelMap.erase(FindIter); // 현재 레벨을 Level 관리구조에서 제외시키고
	GEngine->IsCurLevelReset = true; // 다음 프레임까지 현재 레벨을 살려둔다.

	return 	GEngine->NextLevel;
}

void UEngineCore::SetNextLevel(std::shared_ptr<class ULevel> _NextLevel)
{
	GEngine->NextLevel = _NextLevel;
}

void UEngineCore::DestroyLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	if (false == GEngine->LevelMap.contains(UpperName))
	{
		return;
	}

	std::map<std::string, std::shared_ptr<class ULevel>>::iterator FindIter = GEngine->LevelMap.find(UpperName);

	if (nullptr != FindIter->second)
	{
		FindIter->second = nullptr;
	}

	GEngine->LevelMap.erase(FindIter);
}
