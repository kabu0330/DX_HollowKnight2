#include "PreCompile.h"
#include "TitleGameMode.h"
#include "TitleLogo.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "TitleScene.h"

class TestWindow : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // �� ĭ ����
		ImGui::Text("test");

	}
};

ATitleGameMode::ATitleGameMode()
{
	//UEngineDirectory Dir;
	//if (false == Dir.MoveParentToDirectory("ContentsResources"))
	//{
	//	MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
	//	return;
	//}
	//Dir.Append("Image/Title/TitleBackGround");
	//UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());

	TitleScene = GetWorld()->SpawnActor<ATitleScene>();
	TitleScene->SetActorLocation({ 0.0f, 0.0f, 0.0f });
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({0.0f, 0.0f, -1000.0f, 1.0f});



	//UEngineGUI::CreateGUIWindow<TestWindow>("TestWindow");
}

ATitleGameMode::~ATitleGameMode()
{

}

void ATitleGameMode::BeginPlay()
{

}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

}

