#pragma once
#include <EngineBase/EngineMath.h>

struct UEngineInitData
{
public:
	FVector WindowPos;
	FVector WindowSize;
};

// ���� ������ Ŭ������ ���� �������� ȣ���ϴ� ���
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
		MSGASSERT("������ ��� ������ �����߽��ϴ�."); \
	} \
	return 0; \
}