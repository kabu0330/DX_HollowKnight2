#pragma once
#include <EngineCore/EngineGUIWindow.h>

// Ό³Έν :
class UDebugWindowGUI : public UEngineGUIWindow
{
public:
	UDebugWindowGUI();
	~UDebugWindowGUI();

	void GetMousePos();
	void GetCurRoom();
	void ShowKnightInfo();
	void GetGravityForce();
	void GetFrame();
	void SetFreeCamera();
	void ShowLoadingTime();
	void ShowPlayLoadingTime();


protected:
	void OnGUI() override;

private:

private:
	// delete Function
	UDebugWindowGUI(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI(UDebugWindowGUI&& _Other) noexcept = delete;
	UDebugWindowGUI& operator=(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI& operator=(UDebugWindowGUI&& _Other) noexcept = delete;
};

