#pragma once
#include <EngineCore/EngineGUIWindow.h>

// ���� :
class UDebugWindowGUI : public UEngineGUIWindow
{
public:
	UDebugWindowGUI();
	~UDebugWindowGUI();

	void GetMousePos();
	void GetCurRoom();
	void GetKnightInfo();
	void GetGravityForce();
	void GetFrame();


protected:
	void OnGUI() override;

private:
	// delete Function
	UDebugWindowGUI(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI(UDebugWindowGUI&& _Other) noexcept = delete;
	UDebugWindowGUI& operator=(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI& operator=(UDebugWindowGUI&& _Other) noexcept = delete;
};

