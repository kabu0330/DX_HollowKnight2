#include "PreCompile.h"
#include "Renderer.h"
#include <EngineBase/EngineString.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/Mesh.h>
#include "EngineVertex.h"
#include "EngineBlend.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
}

void URenderer::SetOrder(int _Order)
{
	if (0 != GetOrder() && _Order == GetOrder())
	{
		return;
	}
	int PrevOrder = GetOrder();
	UObject::SetOrder(_Order);
	ULevel* Level = GetActor()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}

ENGINEAPI void URenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
	SetOrder(GetOrder());
}

URenderUnit& URenderer::GetRenderUnit(UINT _Index)
{
	return Units[_Index];
}

void URenderer::SetMesh(std::string_view _Name, UINT _Index /*= 0*/)
{
	URenderUnit& Unit = GetRenderUnit(_Index);
	Unit.SetMesh(_Name);
}

void URenderer::SetMaterial(std::string_view _Name, UINT _Index /*= 0*/)
{
	URenderUnit& Unit = GetRenderUnit(_Index);
	Unit.SetMaterial(_Name);
}

// 자식클래스에서 행렬 값을 바꿀 수 있도록 가상 함수로 만들고 Render에서 분리
void URenderer::RenderTransUpdate(UEngineCamera* _Camera)
{
	// 트랜스폼은 렌더러가 가진다.
	FTransform& RendererTrans = GetTransformRef(); // 렌더러의 트랜스폼

	FTransform& CameraTrans = _Camera->GetTransformRef();
	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;

	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;
}

void URenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	this->RenderTransUpdate(_Camera);

	for (size_t i = 0; i < Units.size(); i++)
	{
		Units[i].Render(_Camera, _DeltaTime);
	}
}

URenderUnit& URenderer::CreateRenderUnit()
{
	//URenderUnit NewRenderUnit;
	//URenderUnit Arr[10];
	//Arr[9] = NewRenderUnit;
	//Units.push_back(NewRenderUnit);

	// emplace_back에 대해서 알아봅시다.
	// 내부에서 Placement new를 통해서 복사를 안해도 되게 최적화를 해주는 문법입니다.
	// 14부터 지원되는 것으로 알고 있습니다.
	// Arr[9];
	// new(&Arr[9]) URenderUnit();

 	URenderUnit& NewUnit = Units.emplace_back();
	NewUnit.ParentRenderer = this;
	return NewUnit;
}