#include "PreCompile.h"
#include "ContentsEditorGUI.h"
#include "EngineCore/EngineCore.h"
#include "EngineCore/EngineGUI.h"
#include "EnginePlatform/EngineSound.h"

UContentsEditorGUI::UContentsEditorGUI()
{
}

UContentsEditorGUI::~UContentsEditorGUI()
{
}

void UContentsEditorGUI::OnGUI()
{
	std::map<std::string, std::shared_ptr<class ULevel>> Map = UEngineCore::GetAllLevels();

	for (std::pair<const std::string, std::shared_ptr<class ULevel>>& Pair : Map)
	{
		if (ImGui::Button(Pair.first.c_str()))
		{
			UEngineSound::TurnOffAllSounds();
			UEngineCore::OpenLevel(Pair.first);
		}
	}
}
