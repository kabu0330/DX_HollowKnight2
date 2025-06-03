#pragma once
#include <EngineCore/EngineGUIWindow.h>

// Ό³Έν :
class UContentsEditorGUI : public UEngineGUIWindow
{
public:
	UContentsEditorGUI();
	~UContentsEditorGUI();

protected:
	void OnGUI() override;

private:
	// delete Function
	UContentsEditorGUI(const UContentsEditorGUI& _Other) = delete;
	UContentsEditorGUI(UContentsEditorGUI&& _Other) noexcept = delete;
	UContentsEditorGUI& operator=(const UContentsEditorGUI& _Other) = delete;
	UContentsEditorGUI& operator=(UContentsEditorGUI&& _Other) noexcept = delete;
};

