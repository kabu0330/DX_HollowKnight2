#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineSound.h>
#include <EnginePlatform/EngineInput.h>
#include "IContentCore.h"
#include "EngineResourceBase.h"
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

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::GetAllLevels()
{
	return GEngine->AllLevels;
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
	GEngine->MainWindow.OpenWindow("MainWindow");
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

	GEngine->ContentDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == GEngine->ContentDLL)
	{
		MSGASSERT("Contents dll ������ �ε��� �� �����ϴ�.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentCore>&) = 
		(INT_PTR(__stdcall*)(std::shared_ptr<IContentCore>&))GetProcAddress(GEngine->ContentDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("������ �ھ� ���ο� CreateContentsCoreDefine�� �������� �ʾҽ��ϴ�.");
		return;
	}

	Ptr(GEngine->Core);

	if (nullptr == GEngine->Core)
	{
		MSGASSERT("������ �ھ� ������ �����߽��ϴ�.");
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

// ��� ��ȯ ������ ���� ���� �Լ��и�
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	if (true == GEngine->AllLevels.contains(_Name.data()))
	{
		MSGASSERT("���� �̸��� ������ �� ������� �����ϴ�." + std::string(_Name.data()));
		return nullptr;
	}

	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(_Name);

	GEngine->AllLevels.insert({ _Name.data(), Ptr}); // ������ ������ ��� LevelMap�� ����

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (false == GEngine->AllLevels.contains(UpperString))
	{
		MSGASSERT(std::string(_Name) + " �� �������� ���� �����Դϴ�. \n CreateLevel �Լ��� ����� ������ ���� �� OpenLevel �Լ��� ����ؾ� �մϴ�.");
		return;
	}

	GEngine->NextLevel = GEngine->AllLevels[UpperString];
}

void UEngineCore::EngineFrame()
{
	// ���� ������ �����Ұž�?
	if (true == GEngine->IsCurLevelReset)
	{
		GEngine->CurLevel = nullptr;
		GEngine->IsCurLevelReset = false;
	}

	if (nullptr != GEngine->NextLevel) // ����ü������ Level�� �����ϸ�
	{
		if (nullptr != GEngine->CurLevel) // ���� ������ ����Ǹ鼭 �� ���� ������ ������ �ϰ�
		{
			GEngine->CurLevel->EndLevel();
		}

		GEngine->CurLevel = GEngine->NextLevel; // ������ �ٲ۴�.

		GEngine->CurLevel->StartLevel(); // ���ο� �������� ó�� ������ �۾��� ���� �����ϰ�
		GEngine->NextLevel = nullptr; // NextLevel �������� ������ ���ߴ�.
		GEngine->Timer.TimeStart(); // ��Ÿ Ÿ�ӵ� ó������ �ٽ� �����Ѵ�. Ȥ�� �� ������ �������
	}

	GEngine->Timer.TimeCheck(); // ��Ÿ Ÿ�� üũ
	float DeltaTime = GEngine->Timer.GetDeltaTime();

	if (true == GEngine->MainWindow.IsFocus()) // �����찡 ��Ŀ���� ������ ����
	{
		UEngineInput::CheckInput(DeltaTime); // Ű �Է� üũ
	}
	else 
	{
		UEngineInput::ResetKey();
	}

	UEngineSound::Update();


	GEngine->CurLevel->Tick(DeltaTime);
	GEngine->CurLevel->Render(DeltaTime);
	GEngine->CurLevel->Collision(DeltaTime);
	GEngine->CurLevel->Release(DeltaTime);
}

void UEngineCore::EndEngine()
{
	UEngineGUI::Release();

	GEngine->Device.Release();

	UEngineResourceBase::Release();
	UEngineConstantBuffer::Release();
	UEngineSound::Release();

	GEngine->CurLevel = nullptr;
	GEngine->NextLevel = nullptr;
	GEngine->AllLevels.clear();

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
		DestroyLevel(_LevelName); // �����
		return false;
	}
	return true;
}

std::shared_ptr<ULevel> UEngineCore::ReadyToNextLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	std::map<std::string, std::shared_ptr<ULevel>>::iterator FindIter = GEngine->AllLevels.find(UpperName);
	GEngine->AllLevels.erase(FindIter); // ���� ������ Level ������������ ���ܽ�Ű��
	GEngine->IsCurLevelReset = true; // ���� �����ӱ��� ���� ������ ����д�.

	return 	GEngine->NextLevel;
}

void UEngineCore::SetNextLevel(std::shared_ptr<class ULevel> _NextLevel)
{
	GEngine->NextLevel = _NextLevel;
}

void UEngineCore::DestroyLevel(std::string_view _LevelName)
{
	std::string UpperName = UEngineString::ToUpper(_LevelName);

	if (false == GEngine->AllLevels.contains(UpperName))
	{
		return;
	}

	std::map<std::string, std::shared_ptr<class ULevel>>::iterator FindIter = GEngine->AllLevels.find(UpperName);

	if (nullptr != FindIter->second)
	{
		FindIter->second = nullptr;
	}

	GEngine->AllLevels.erase(FindIter);
}
