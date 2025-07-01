#include "PreCompile.h"
#include "Level.h"
#include "Actor.h"
#include "Renderer.h"
#include "Collision.h"
#include "EngineCore.h"
#include "EngineCamera.h"
#include "CameraActor.h"
#include "EngineGUI.h"
#include "EngineFont.h"
#include "EngineRenderTarget.h"
#include "HUD.h"

std::shared_ptr<class ACameraActor> ULevel::SpawnCamera(int _Order)
{
	std::shared_ptr<ACameraActor> Camera = std::make_shared<ACameraActor>();

	if (true == AllCameras.contains(_Order))
	{
		MSGASSERT("�̹� �����ϴ� ī�޶� Order�Դϴ�. �ٸ� ���� �Է����ּ���. : " + std::to_string(_Order));
		return nullptr;
	}

	Camera->BeginPlay(); 

	AllCameras.insert({ _Order , Camera });
	return Camera;
}

ULevel::ULevel()
{
	SpawnCamera(EEngineCameraType::MainCamera); // ����ī�޶�

	std::shared_ptr<ACameraActor> UICamera = SpawnCamera(EEngineCameraType::UICamera); // UI ī�޶�
	UICamera->GetCameraComponent()->SetProjectionType(EProjectionType::Orthographic); // UIī�޶�� �������� ���Ѵ�.

	// ȭ�鿡 �׷��� ���� ����Ÿ���� �����.
	LastRenderTarget = std::make_shared<UEngineRenderTarget>();
	LastRenderTarget->CreateRenderTargetView(UEngineCore::GetScreenScale());
	LastRenderTarget->CreateDepthTexture();
}

ULevel::~ULevel()
{
	BeginPlayList.clear();
	AllActors.clear();
	AllCameras.clear();
}

void ULevel::StartLevel()
{
	for (std::shared_ptr<class AActor> Actor : BeginPlayList)
	{
		Actor->StartLevel();
	}


	for (std::shared_ptr<class AActor> Actor : AllActors)
	{
		Actor->StartLevel();
	}
}

void ULevel::EndLevel()
{
	for (std::shared_ptr<class AActor> Actor : BeginPlayList)
	{
		Actor->EndLevel();
	}

	for (std::shared_ptr<class AActor> Actor : AllActors)
	{
		Actor->EndLevel();
	}
}

void ULevel::Tick(float _DeltaTime)
{
	if (GetMainCamera()->IsFreeCamera()) // ���� ī�޶� �Ѹ� ������ Tick�� �Ͻ�����
	{
		return;
	}

	// Tick�� ������ ����, �ش� �����ӿ� ������ Actor�� BeginPlay�� ���� ȣ���Ѵ�.
	std::list<std::shared_ptr<class AActor>>::iterator StartIter = BeginPlayList.begin();
	std::list<std::shared_ptr<class AActor>>::iterator EndIter = BeginPlayList.end();
	for (; StartIter != EndIter; )
	{
		std::shared_ptr<AActor> CurActor = *StartIter;

		if (false == CurActor->IsActive()) // ��Ȱ��ȭ�� ���ʹ� �ϴ� ����
		{
			++StartIter;
			continue;
		}

		StartIter = BeginPlayList.erase(StartIter); // BeginPlay�� ȣ��� ���ʹ� �ش� ����Ʈ���� �����Ѵ�.
		// ���� �����ӿ��� BeginPlay�� �� ȣ��� ���� ����.

		CurActor->BeginPlay(); // ȣ��

		if (nullptr != CurActor->Parent)
		{
			continue;
		}

		AllActors.push_back(CurActor); // BeginPlay�� ȣ��� Actor�� Tick�� ���� ActorList�� ����.
	}

	// BeginPlay�� ȣ��� Actor
	for (std::shared_ptr<AActor> CurActor : AllActors)
	{
		if (false == CurActor->IsActive()) // ��Ȱ��ȭ�� Actor�� Tick�� ������ �ʴ´�.
		{
			continue;
		}

		CurActor->Tick(_DeltaTime);
	}
}

void ULevel::Render(float _DeltaTime)
{
	UEngineCore::GetDevice().StartRender(); // ����� �ʱ�ȭ �� OM�ܰ迡�� ����� RTV�� DSV ����

	LastRenderTarget->ClearRenderTargets(); // ���� ��� ȭ�鵵 ȭ�� �� �� ����

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : AllCameras)
	{
		if (Camera.first == static_cast<int>(EEngineCameraType::UICamera)) 	// UIī�޶�� ���� ������ �����ش�.
		{
			continue;
		}

		if (false == Camera.second->IsActive())
		{
			continue;
		}

		Camera.second->Tick(_DeltaTime); // View ��İ� Projection ��� ���
		Camera.second->GetCameraComponent()->Render(_DeltaTime);

		Camera.second->GetCameraComponent()->CameraTarget->MergeToRenderTarget(LastRenderTarget); // ������ ��������������
	}

	if (true == AllCameras.contains(static_cast<int>(EEngineCameraType::UICamera))) // UIī�޶�� ���� ������ �����ش�.
	{
		std::shared_ptr<ACameraActor> CameraActor = AllCameras[static_cast<int>(EEngineCameraType::UICamera)];
		if (true == CameraActor->IsActive()) // UIī�޶� ��Ƽ�� �����϶��� ������.
		{
			std::shared_ptr<UEngineCamera> CameraComponent = AllCameras[static_cast<int>(EEngineCameraType::UICamera)]->GetCameraComponent();

			CameraActor->Tick(_DeltaTime); // ƽ�� ������
			CameraComponent->CameraTarget->ClearRenderTargets(); // ȭ�鵵 �����
			CameraComponent->CameraTarget->OMSetRenderTargets(); // ����Ÿ�� �����ϰ�

			HUD->UIRender(CameraComponent.get(), _DeltaTime); // ������ ƽ, ���� ������

			CameraComponent->CameraTarget->MergeToRenderTarget(LastRenderTarget); // ���� ��º����ϰ�
		}
	}
	else
	{
		MSGASSERT("UIī�޶� �������� �ʽ��ϴ�. UIī�޶� ������ �ּ���.");
		return;
	}

	// ����� ���� Ÿ�� ��� ����
	std::shared_ptr<UEngineRenderTarget> BackBuffer = UEngineCore::GetDevice().GetBackBufferTarget();
	LastRenderTarget->MergeToRenderTarget(BackBuffer);

	// ����� ������ : �ַ� �ݸ���
	{
		std::shared_ptr<class ACameraActor> Camera = GetMainCamera();

		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : AllCollisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			for (std::shared_ptr<UCollision>& _Collision : List)
			{
				if (false == _Collision->IsActive())
				{
					continue;
				}

				_Collision->ApplyDebugRender(Camera->GetCameraComponent().get(), _DeltaTime); // �����
			}
		}
	}

	// IMGUI ������
	if (true == UEngineWindow::IsLoopActivate())
	{
		UEngineGUI::GUIRender(this);
	}

	// Present
	UEngineCore::GetDevice().EndRender(); // ����ü���� �����ϴ� ����ۿ� ����Ʈ���۸� ��ȯ(Swap) 
	// ����Ʈ����(������ â)�� ���
}

void ULevel::ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	if (false == AllCameras.contains(_CameraOrder))
	{
		MSGASSERT("�������� �ʴ� ī�޶� �ε����Դϴ�.");
		return;
	}

	std::shared_ptr<ACameraActor> Camera = AllCameras[_CameraOrder];

	Camera->GetCameraComponent()->ChangeRenderGroup(_PrevGroupOrder, _Renderer);
}

void ULevel::CreateCollisionProfile(std::string_view _ProfileName)
{
	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	AllCollisions[UpperName];
}

void ULevel::LinkCollisionProfile(std::string_view _LeftProfileName, std::string_view _RightProfileName)
{
	std::string LeftUpperName = UEngineString::ToUpper(_LeftProfileName);
	std::string RightUpperName = UEngineString::ToUpper(_RightProfileName);

	CollisionLinks[LeftUpperName].push_back(RightUpperName);
}

void ULevel::ChangeCollisionProfile(std::string_view _ProfileName, std::string_view _PrevProfileName, std::shared_ptr<UCollision> _Collision)
{
	if (false == AllCollisions.contains(_ProfileName.data()))
	{
		MSGASSERT("�ݸ��� �������� �������� �ʽ��ϴ�. �������� �������ּ���.");
		return;
	}

	std::string PrevUpperName = UEngineString::ToUpper(_PrevProfileName);

	if (_PrevProfileName != "")
	{
		std::list<std::shared_ptr<UCollision>>& PrevCollisionGroup = AllCollisions[PrevUpperName];
		PrevCollisionGroup.remove(_Collision);
	}

	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	std::list<std::shared_ptr<UCollision>>& CollisionGroup = AllCollisions[UpperName];
	CollisionGroup.push_back(_Collision);
}

void ULevel::Collision(float _DeltaTime)
{
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinks)
	{
		const std::string& LeftProfile = Links.first;

		std::list<std::string>& LinkSecond = Links.second;

		for (std::string& RightProfile : LinkSecond)
		{
			std::list<std::shared_ptr<class UCollision>>& LeftList = AllEventCollisions[LeftProfile];
			std::list<std::shared_ptr<class UCollision>>& RightList = AllCollisions[RightProfile];

			for (std::shared_ptr<class UCollision>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<class UCollision>& RightCollision : RightList)
				{
					if (false == LeftCollision->IsActive())
					{
						continue;
					}

					LeftCollision->CheckCollisionEvent(RightCollision);
				}
			}
		}
	}
}

void ULevel::Release(float _DeltaTime)
{
	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : AllCameras)
	{
		Camera.second->GetCameraComponent()->Release(_DeltaTime);
	}

	{
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : AllCollisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			std::list<std::shared_ptr<UCollision>>::iterator StartIter = List.begin();
			std::list<std::shared_ptr<UCollision>>::iterator EndIter = List.end();

			for (; StartIter != EndIter; )
			{
				if (false == (*StartIter)->IsDestroy())
				{
					++StartIter;
					continue;
				}

				StartIter = List.erase(StartIter);
			}
		}
	}

	{
		std::list<std::shared_ptr<AActor>>& List = AllActors;

		std::list<std::shared_ptr<AActor>>::iterator StartIter = List.begin();
		std::list<std::shared_ptr<AActor>>::iterator EndIter = List.end();

		for (; StartIter != EndIter; )
		{
			if (nullptr != (*StartIter)->Parent)
			{
				StartIter = List.erase(StartIter);
				continue;
			}

			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = List.erase(StartIter);
		}
	}
}

void ULevel::InitLevel(AGameMode* _GameMode, APawn* _Pawn, AHUD* _HUD)
{
	GameMode = _GameMode;
	MainPawn = _Pawn;
	HUD = _HUD;
}