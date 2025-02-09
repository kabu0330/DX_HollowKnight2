#include "PreCompile.h"
#include "Barrier.h"

ABarrier::ABarrier()
{
	SetName("ABarrier");

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<UContentsRenderer>().get();
	Renderer->SetupAttachment(RootComponent);
	Renderer->SetAutoScaleRatio(1.0f);
	ZSort = static_cast<float>(EZOrder::BARRIER);
	Renderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	Collision = CreateDefaultSubObject<UCollision>().get();
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("Barrier");
	Collision->GetTransformRef().Location.Z = ZSort;
	Scale = FVector(30, 300);
	Collision->SetScale3D(Scale);
	//BodyCollision->SetCollisionType(ECollisionType::AABB);

	Collision->SetCollisionStay(std::bind(&ABarrier::Barrier, this, std::placeholders::_1, std::placeholders::_2));
	Collision->SetCollisionEnd(std::bind(&ABarrier::BarrierEnd, this, std::placeholders::_1, std::placeholders::_2));

	std::string Barrier = "Barrier";
	std::string On = "On";
	std::string Off = "Off";
	std::string Png = ".png";
	float FrameTime = 0.2f;
	Renderer->CreateAnimation(Barrier + On, Barrier + Png, 0, 4, FrameTime, false);
	Renderer->CreateAnimation(Barrier + Off, Barrier + Png, 5, 9, FrameTime, false);
	Renderer->ChangeAnimation(Barrier + On);
	Renderer->SetActive(true); 
}

ABarrier::~ABarrier()
{
}

void ABarrier::SetActiveBarrier(bool _Value)
{
	if (true == _Value) // ���� ���� ������ ��
	{
		if (nullptr != Collision)
		{
			Collision->SetActive(true);
		}
		if (nullptr != Renderer)
		{
			Renderer->SetActive(true);
			// �ִϸ��̼� ����� ����� �Ѵ�.
		}
	}
	else // ���� ���� �ƴϰų� ������ ��
	{
		// �������� ���߿� ��Ƽ�긦 ���� �ϴ� �ø����� ���� ����.
		Collision->SetActive(false);
	}
}

void ABarrier::BeginPlay()
{
	AActor::BeginPlay();
}

void ABarrier::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

}

void ABarrier::Barrier(UCollision* _This, UCollision* _Other)
{
	AActor* Actor = _Other->GetActor();
	
	AKnight* Knight = dynamic_cast<AKnight*>(Actor);
	if (nullptr != Knight)
	{
		Knight->SetBarrier(true);
		Knight->GetBarrierPos(GetActorLocation());
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(Actor);
	if (nullptr != Monster)
	{
		Monster->SetBarrier(true);
		Monster->GetBarrierPos(GetActorLocation());
		return;
	}
}

void ABarrier::BarrierEnd(UCollision* _This, UCollision* _Other)
{
	AActor* Actor = _Other->GetActor();

	AKnight* Knight = dynamic_cast<AKnight*>(Actor);
	if (nullptr != Knight)
	{
		Knight->SetBarrier(false);
		Knight->GetBarrierPos(FVector::ZERO);
		return;
	}

	AMonster* Monster = dynamic_cast<AMonster*>(Actor);
	if (nullptr != Monster)
	{
		Monster->SetBarrier(false);
		Monster->GetBarrierPos(FVector::ZERO);
		return;
	}
}

