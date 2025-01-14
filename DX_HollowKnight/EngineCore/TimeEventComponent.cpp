#include "PreCompile.h"
#include "TimeEventComponent.h"

UTimeEventComponent::UTimeEventComponent()
{
}

UTimeEventComponent::~UTimeEventComponent()
{
}

void UTimeEventComponent::AddEndEvent(float _Time, 	// 5초가 끝날때 이 함수를 호출해라
	std::function<void()> _TimeEndEvent, bool _Loop
)
{
	AddEvent(_Time, nullptr, _TimeEndEvent, _Loop);
}

void UTimeEventComponent::AddUpdateEvent(float _Time, std::function<void(float, float)> _TimeUpdateEvent, bool _Loop
)
{
	AddEvent(_Time, _TimeUpdateEvent, nullptr, _Loop);
}

void UTimeEventComponent::AddEvent(float _Time, std::function<void(float, float)> _TimeUpdateEvent, std::function<void()> _TimeEndEvent, bool _Loop /*= false*/)
{
	// 기본 생성자로 새로운 원소를 바로 만들어내는 
	// 기본 생성자가 없는 클래스는 만들수가 없다.
	// 더 정확하게 이야기하자면 템플릿가변인자를 통해서 생성자에 무작위 패턴에 대응할수 있게 만들어져 있는데.
	FTimeEvent& NewEvent = Events.emplace_back();
	NewEvent.MaxTime = _Time;
	NewEvent.CurTime = 0.0f;
	NewEvent.Loop = _Loop;
	NewEvent.TimeEndEvent = _TimeEndEvent;
	NewEvent.TimeUpdateEvent = _TimeUpdateEvent;

};

void UTimeEventComponent::BeginPlay()
{

}

void UTimeEventComponent::ComponentTick(float _DeltaTime)
{
	UActorComponent::ComponentTick(_DeltaTime);

	for (FTimeEvent& TimeEvent : Events)
	{
		TimeEvent.CurTime += _DeltaTime;

		if (nullptr != TimeEvent.TimeUpdateEvent)
		{
			TimeEvent.TimeUpdateEvent(_DeltaTime, TimeEvent.CurTime);
		}

		if (TimeEvent.CurTime > TimeEvent.MaxTime)
		{
			if (nullptr == TimeEvent.TimeEndEvent)
			{
				continue;
			}
			TimeEvent.TimeEndEvent();
		}
	}

	// 삭제연산할때 ranged for는 사용하지 않는다. 

	std::list<FTimeEvent>::iterator StartIter = Events.begin();
	std::list<FTimeEvent>::iterator EndIter = Events.end();

	for ( ; StartIter != EndIter; )
	{
		FTimeEvent& TimeEvent = *StartIter;

		if (TimeEvent.CurTime < TimeEvent.MaxTime)
		{
			++StartIter;
			continue;
		}

		if (false == TimeEvent.Loop)
		{
			StartIter = Events.erase(StartIter);
		}
		else 
		{
			++StartIter;
			TimeEvent.CurTime = 0.0f;
		}
	}

}








