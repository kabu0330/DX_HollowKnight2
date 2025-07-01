#pragma once

#include <EngineBase/Object.h>
#include <EngineBase/EngineDebug.h>

// 설명 :
class ULevel : public UObject
{
	friend class UCollision;
	friend class UEngineCore;

public:
	ENGINEAPI ULevel();
	ENGINEAPI ~ULevel();

	void StartLevel();
	void EndLevel();

	template<typename Type>
	Type* GetGameMode()
	{
		return dynamic_cast<Type*>(GameMode);
	}
	class AGameMode* GetGameMode()
	{
		return GameMode;
	}

	class APawn* GetMainPawn()
	{
		return MainPawn;
	}
	template<typename Type>
	Type* GetMainPawn()
	{
		return dynamic_cast<Type*>(MainPawn);
	}

	class AHUD* GetHUD()
	{
		return HUD;
	}
	template<typename Type>
	Type* GetHUD()
	{
		return dynamic_cast<Type*>(HUD);
	}


	void Tick(float _DeltaTime);
	void Render(float _DeltaTime);
	void Collision(float _DeltaTime);
	void Release(float _DeltaTime);

	std::shared_ptr<class ACameraActor> GetMainCamera()
	{
		return GetCamera(0);
	}

	template<typename EnumType>
	std::shared_ptr<class ACameraActor> GetCamera(EnumType _Order)
	{
		return GetCamera(static_cast<int>(_Order));
	}

	std::shared_ptr<class ACameraActor> GetCamera(int _Order)
	{
		if (false == AllCameras.contains(_Order))
		{
			MSGASSERT("존재하지 않는 카메라 인덱스입니다.");
		}

		return AllCameras[_Order];
	}

	template<typename EnumType>
	std::shared_ptr<class ACameraActor> SpawnCamera(EnumType _Order)
	{
		return SpawnCamera(static_cast<int>(_Order));
	}

	std::shared_ptr<class ACameraActor> SpawnCamera(int _Order);

	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor(std::string_view _Name = "")
	{
		static_assert(std::is_base_of_v<AActor, ActorType>, "[타입에러] 액터를 상속한 대상만 Spawn할 수 있습니다.");

		if (false == std::is_base_of_v<AActor, ActorType>)
		{
			MSGASSERT("[타입에러] 액터를 상속한 대상만 Spawn할 수 있습니다.");
			return nullptr;
		}

		char* ActorMemory = new char[sizeof(ActorType)];

		AActor* ActorPtr = reinterpret_cast<ActorType*>(ActorMemory);
		ActorPtr->World = this;

		ActorType* NewPtr = reinterpret_cast<ActorType*>(ActorMemory);

		std::shared_ptr<ActorType> NewActor(NewPtr = new(ActorMemory) ActorType());

		ActorPtr->SetName(_Name);

		BeginPlayList.push_back(NewActor);

		return NewActor;
	}

	ENGINEAPI void ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<class URenderer> _Renderer);

	ENGINEAPI void CreateCollisionProfile(std::string_view _ProfileName);
	ENGINEAPI void ChangeCollisionProfile(std::string_view _ProfileName, std::string_view _PrevProfileName, std::shared_ptr<class UCollision> _Collision);
	ENGINEAPI void LinkCollisionProfile(std::string_view _LeftProfileName, std::string_view _RightProfileName);
	ENGINEAPI void PushCollisionProfileEvent(std::shared_ptr<class URenderer> _Renderer) {};


	template<typename ConvertType>
	ENGINEAPI std::list<std::shared_ptr<ConvertType>> GetAllActorListByClass()
	{
		std::list<std::shared_ptr<ConvertType>> List;

		for (std::shared_ptr<class AActor> Actor : AllActors)
		{
			std::shared_ptr<ConvertType> Convert = std::dynamic_pointer_cast<ConvertType>(Actor);
			if (nullptr == Convert)
			{
				continue;
			}
			List.push_back(Convert);
		}

		return List;
	}

	template<typename ConvertType>
	ENGINEAPI std::vector<std::shared_ptr<ConvertType>> GetAllActorArrayByClass()
	{
		std::vector<std::shared_ptr<ConvertType>> List;

		for (std::shared_ptr<class AActor> Actor : AllActors)
		{
			std::shared_ptr<ConvertType> Convert = std::dynamic_pointer_cast<ConvertType>(Actor);
			if (nullptr == Convert)
			{
				continue;
			}
			List.push_back(Convert);
		}

		return List;
	}

	ENGINEAPI class UEngineRenderTarget* GetLastRenderTarget()
	{
		return LastRenderTarget.get();
	}

protected:

private:
	class AHUD* HUD = nullptr;

	class AGameMode* GameMode = nullptr;
	class APawn* MainPawn = nullptr;


	std::list<std::shared_ptr<class AActor>> BeginPlayList;

	std::list<std::shared_ptr<class AActor>> AllActors;

	std::map<int, std::shared_ptr<class ACameraActor>> AllCameras;

	// 모든 카메라(Play 화면 + UI 등)가 바라본 이미지를 섞은 타겟 => 백버퍼로 넘길 최종 타겟
	std::shared_ptr<class UEngineRenderTarget> LastRenderTarget;

	std::map<std::string, std::list<std::shared_ptr<class UCollision>>> AllCollisions;

	// 이벤트가 존재하는 애들만 충돌 체크하려고.
	std::map<std::string, std::list<std::shared_ptr<class UCollision>>> AllEventCollisions;

	std::map<std::string, std::list<std::string>> CollisionLinks;

	std::map<int, std::list<std::shared_ptr<class UWidget>>> Widgets;

	ENGINEAPI void InitLevel(AGameMode* _GameMode, APawn* _Pawn, AHUD* _HUD);

private:
	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;
};

