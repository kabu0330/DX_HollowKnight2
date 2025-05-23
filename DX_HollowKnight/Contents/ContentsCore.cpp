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
#include "TitleHUD.h"
#include "PlayHUD.h"

// 상위 엔진 레벨에 해당 클래스를 EngineCore에 간접적인 제어권한을 가지는 컨텐츠 코어로 설정한다.
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
	_Data.WindowPos = { 50,0 };
	_Data.WindowSize = { 1280, 720 };
}

void UContentsCore::CreateLevel()
{
	UEngineCore::CreateLevel<ATitleGameMode, APawn, ATitleHUD>("Title");
	UEngineCore::CreateLevel<APlayGameMode, AKnight, APlayHUD>("Play");

#ifdef _DEBUG
	UEngineCore::OpenLevel("Play");
#else
	UEngineCore::OpenLevel("Title");
#endif // _DEBUG
}

void UContentsCore::EngineTick(float _DeltaTime)
{
}

void UContentsCore::EngineEnd()
{

}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}