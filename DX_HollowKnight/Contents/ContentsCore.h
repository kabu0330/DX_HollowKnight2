#pragma once
#include <EngineCore/IContentCore.h>

// ���� : ������ ũ�� ����, ���ҽ� �ε�, ���� ����
class UContentsCore : public IContentCore
{
public:
	UContentsCore();
	~UContentsCore();

	class UDebugWindowGUI* GetDebugGUI() const
	{
		return DebugGUI.get();
	}

protected:
	void StartEngine(UEngineInitData& _Data);
	void EngineTick(float _DeltaTime);
	void EndEngine();

private:
	void SetWindowSize(UEngineInitData& _Data);

	void CreateLevel();
	void CreateGUI();

	std::shared_ptr<class UDebugWindowGUI> DebugGUI = nullptr;

private:
	// delete Function
	UContentsCore(const UContentsCore& _Other) = delete;
	UContentsCore(UContentsCore&& _Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& _Other) = delete;
	UContentsCore& operator=(UContentsCore&& _Other) noexcept = delete;
};

