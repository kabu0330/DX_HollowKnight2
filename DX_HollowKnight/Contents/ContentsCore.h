#pragma once
#include <EngineCore/IContentsCore.h>

// 설명 : 윈도우 크기 설정, 리소스 로드, 레벨 생성
class UContentsCore : public IContentsCore
{
public:
	UContentsCore();
	~UContentsCore();

protected:
	void EngineStart(UEngineInitData& _Data);
	void EngineTick(float _DeltaTime);
	void EngineEnd();

private:
	void SetWindowSize(UEngineInitData& _Data);

	void CreateLevel();


private:
	// delete Function
	UContentsCore(const UContentsCore& _Other) = delete;
	UContentsCore(UContentsCore&& _Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& _Other) = delete;
	UContentsCore& operator=(UContentsCore&& _Other) noexcept = delete;
};

