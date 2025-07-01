#include "PreCompile.h"
#include "ContentsCore.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/HUD.h>

#include "ContentsResource.h"
#include "TitleGameMode.h"
#include "PlayGameMode.h"
#include "MapEditorGameMode.h"
#include "ContentsEditorGUI.h"
#include "DebugWindowGUI.h"
#include "TitleHUD.h"
#include "PlayHUD.h"

// ���� ���� ������ �ش� Ŭ������ EngineCore�� �������� ��������� ������ ������ �ھ�� �����Ѵ�.
CreateContentsCoreDefine(UContentsCore);

void UContentsCore::StartEngine(UEngineInitData& _Data)
{
	SetWindowSize(_Data);

	UContentsResource::LoadShaderResource();

	CreateLevel();
	CreateGUI();
}

void UContentsCore::SetWindowSize(UEngineInitData& _Data)
{
	_Data.WindowPos = { 50,0 };
	_Data.WindowSize = { 1280, 720 };
}

void UContentsCore::CreateLevel()
{
	UEngineCore::CreateLevel<ATitleGameMode, APawn, ATitleHUD>("Title");

#ifdef _DEBUG
	//UEngineCore::OpenLevel("Play");
	UEngineCore::OpenLevel("Title");
#else
	UEngineCore::OpenLevel("Title");
#endif // _DEBUG
}

void UContentsCore::CreateGUI()
{
	UEngineGUI::DeactivateAllWindow();

	//{
	//	UEngineGUI::CreateGUIWindow<UContentsEditorGUI>("ContentEditorGUI");
	//	std::shared_ptr<UContentsEditorGUI> Window = UEngineGUI::FindGUIWindow<UContentsEditorGUI>("ContentEditorGUI");
	//	Window->SetActive(true);
	//}

	{
		DebugGUI = UEngineGUI::FindGUIWindow<UDebugWindowGUI>("DebugWindow");
		if (nullptr == DebugGUI)
		{
			DebugGUI = UEngineGUI::CreateGUIWindow<UDebugWindowGUI>("DebugWindow");
		}
		DebugGUI->SetActive(true);
	}
}

void UContentsCore::EngineTick(float _DeltaTime)
{
}

void UContentsCore::EndEngine()
{

}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}