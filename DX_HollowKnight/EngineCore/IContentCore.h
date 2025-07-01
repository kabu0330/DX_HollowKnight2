#pragma once
#include <EngineBase/EngineMath.h>

struct UEngineInitData
{
public:
	FVector WindowPos;
	FVector WindowSize;
};

// 하위 계층의 클래스를 상위 계층에서 호출하는 방법
class IContentCore
{
public:
	virtual ~IContentCore() = 0 {};

	virtual void StartEngine(UEngineInitData& _Data) = 0 {};
	virtual void EngineTick(float _DeltaTime) {};
	virtual void EndEngine() {};
};

#define CreateContentsCoreDefine(TYPE) \
STDAPI_(__declspec(dllexport) INT_PTR) __stdcall CreateContentsCore(std::shared_ptr<IContentCore>& _Core) \
{ \
	_Core = std::make_shared<TYPE>(); \
	if (nullptr == _Core) \
	{ \
		MSGASSERT("컨텐츠 모듈 생성에 실패했습니다."); \
	} \
	return 0; \
}