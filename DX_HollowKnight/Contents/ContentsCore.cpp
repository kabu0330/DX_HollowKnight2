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

// ���� ���� ������ �ش� Ŭ������ EngineCore�� �������� ��������� ������ ������ �ھ�� �����Ѵ�.
CreateContentsCoreDefine(UContentsCore);

void UContentsCore::EngineStart(UEngineInitData& _Data)
{
	SetWindowSize(_Data);

	UContentsResource::LoadResourceDirectory();
	UContentsResource::LoadFolder();
	UContentsResource::LoadSprite();
	UContentsResource::LoadShaderResource();

	CreateLevel();

	UEngineGUI::AllWindowOff();

	UEngineGUI::CreateGUIWindow<UContentsEditorGUI>("ContentsEditorGUI");
	std::shared_ptr<UContentsEditorGUI> Window = UEngineGUI::FindGUIWindow<UContentsEditorGUI>("ContentsEditorGUI");
	Window->SetActive(true);
}

void UContentsCore::SetWindowSize(UEngineInitData& _Data)
{
	_Data.WindowPos = { 30, 50 };
	_Data.WindowSize = { 1280, 720 };
}

void UContentsCore::CreateLevel()
{
	//UEngineCore::CreateLevel<ATitleGameMode, APawn, AHUD>("Title");
	UEngineCore::CreateLevel<APlayGameMode, AKnight, AHUD>("Play");
	//UEngineCore::CreateLevel<AMapEditorGameMode, APawn, AHUD>("MapEditorMode");
	//UEngineCore::CreateLevel<AMapEditorMode, APawn>("MapEditor");

	//UEngineCore::OpenLevel("Title");
	//UEngineCore::OpenLevel("MapEditor");
	UEngineCore::OpenLevel("Play");
	//UEngineCore::OpenLevel("MapEditorMode");
}

void UContentsCore::EngineTick(float _DeltaTime)
{
	CheckInput();
	SwitchGameMode();
}

void UContentsCore::EngineEnd()
{

}

void UContentsCore::CheckInput()
{
	//for (int i = 0; i < 4; i++)
	//{
	//	if (true == UEngineInput::IsDown('1' + i))
	//	{
	//		Select = i + 1;
	//		break;
	//	}
	//}
}

void UContentsCore::SwitchGameMode()
{
	//switch (Select)
	//{
	//case 0:
	//{
	//	break;
	//}
	//default:
	//	break;
	//}
}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}