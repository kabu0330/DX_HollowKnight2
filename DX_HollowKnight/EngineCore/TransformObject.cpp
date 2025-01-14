#include "PreCompile.h"
#include "TransformObject.h"

UTransformObject::UTransformObject()
{
}

UTransformObject::~UTransformObject()
{
}

void UTransformObject::SetupAttachment(std::shared_ptr<UTransformObject> _Parent)
{
	SetupAttachment(_Parent.get());
}

void UTransformObject::ParentMatrixCheck()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}

void UTransformObject::SetupAttachment(UTransformObject* _Parent)
{
	Parent = _Parent;
	Parent->Childs.push_back(this);

	TransformUpdate();
}

void UTransformObject::TransformUpdate()
{
	ParentMatrixCheck();
	// 나의 트랜스폼 업데이트는 일단 한다.
	Transform.TransformUpdate(IsAbsolute);

	for (UTransformObject* Child : Childs)
	{
		Child->TransformUpdate();
	}

	IsAbsolute = false;
}

