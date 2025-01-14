#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineCamera.h>

// 설명 :
class AHUD : public AActor
{
	friend class ULevel;
public:
	// constrcuter destructer
	ENGINEAPI AHUD();
	ENGINEAPI ~AHUD();

	// delete Function
	AHUD(const AHUD& _Other) = delete;
	AHUD(AHUD&& _Other) noexcept = delete;
	AHUD& operator=(const AHUD& _Other) = delete;
	AHUD& operator=(AHUD&& _Other) noexcept = delete;

	template<typename WidgetType>
	std::shared_ptr<WidgetType> CreateWidget(int _ZOrder, std::string_view _Name = "NONE")
	{
		// 컴파일시점에서 오류가 납니다.
		static_assert(std::is_base_of_v<UWidget, WidgetType>, "액터를 상속받지 않은 클래스를 SpawnActor하려고 했습니다.");

		if (false == std::is_base_of_v<UWidget, WidgetType>)
		{
			MSGASSERT("위젯을 상속받지 않은 클래스를 CreateWidget하려고 했습니다.");
			return nullptr;
			// static_assert
		}

		// 액터와 동일하게 placemenet new를 통해서 생성 전에 값을 미리 세팅하는 방식을 사용.
		char* Memory = new char[sizeof(WidgetType)];

		UWidget* WidgetPtr = reinterpret_cast<WidgetType*>(Memory);
		WidgetPtr->HUD = this;

		WidgetType* NewPtr = reinterpret_cast<WidgetType*>(Memory);
		// 레벨먼저 세팅하고
		// 플레이스먼트 new 
		std::shared_ptr<WidgetType> NewWidgetPtr(NewPtr = new(Memory) WidgetType());

		NewWidgetPtr->SetName(_Name);
		NewWidgetPtr->SetOrder(_ZOrder);

		//컴파일러는 그걸 모른다.
		Widgets[NewWidgetPtr->GetOrder()].push_back(NewWidgetPtr);

		return NewWidgetPtr;
	}

protected:

private:
	std::map<int, std::list<std::shared_ptr<class UWidget>>> Widgets;

	void UIRender(UEngineCamera* _Camera, float _DeltaTime);
};

