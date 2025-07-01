#include "PreCompile.h"
#include "DebugWindowGUI.h"
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include "PlayGameMode.h"
#include "Room.h"
#include "EnginePlatform/EngineInput.h"
#include <EngineCore/EngineRenderTarget.h>
#include "TitleGameMode.h"
#include <EngineCore/EngineCore.h>

UDebugWindowGUI::UDebugWindowGUI()
{
}

UDebugWindowGUI::~UDebugWindowGUI()
{
}

void UDebugWindowGUI::OnGUI()
{
	ShowLoadingTime();
	ShowPlayLoadingTime();

	SetFreeCamera();

	GetFrame();
	GetCurRoom();
	GetMousePos();
	ShowKnightInfo();
	GetGravityForce();
}

void UDebugWindowGUI::SetFreeCamera()
{
	if (true == ImGui::Button("FreeCamera"))
	{
		GetWorld()->GetMainCamera()->SwitchFreeCamera();
	}

	if (UEngineInput::IsDown(VK_F8))
	{
		GetWorld()->GetMainCamera()->SwitchFreeCamera();
		if (true == GetWorld()->GetMainCamera()->IsFreeCamera())
		{
			GetWorld()->GetGameMode<APlayGameMode>()->GetRoomsRef().SetActivate(true);
			GetWorld()->GetLastRenderTarget()->SetClearColor(float4(0.0f, 0.0f, 1.0f, 1.0f));
		}
		else
		{
			GetWorld()->GetLastRenderTarget()->SetClearColor(float4(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}
}

void UDebugWindowGUI::GetMousePos()
{
	FVector MousePos = APlayGameMode::MousePos;
	ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);

	//FVector MapPos = ARoom::GetCurRoom()->GetLeftTopPos();
	//ImGui::Text("Map Pos X : %.0f, Y : %.0f", MousePos.X - MapPos.X, MousePos.Y - MapPos.Y);
}

void UDebugWindowGUI::GetCurRoom()
{
	if (nullptr == ARoom::GetCurRoom())
	{
		return;
	}
	std::string CurRoomName = ARoom::GetCurRoom()->GetName();
	ImGui::Text("CurRoom Name : %s", CurRoomName.c_str());
}

void UDebugWindowGUI::ShowKnightInfo()
{
	AKnight* Knight = AKnight::GetPawn();
	if (nullptr == Knight)
	{
		return;
	}
	FVector KnightPos = APlayGameMode::KnightPos;
	ImGui::Text("Knight Pos X : %.0f, Y : %.0f", KnightPos.X, KnightPos.Y);

	ImGui::Text("Knight MP : %d", Knight->GetStatRef().GetMp());
}

void UDebugWindowGUI::GetGravityForce()
{
	AKnight* Knight = AKnight::GetPawn();
	if (nullptr == Knight)
	{
		return;
	}
	int Result = static_cast<int>(Knight->IsOnGround());
	std::string ResultString = "";
	if (0 == Result)
	{
		ResultString = "false";
	}
	else
	{
		ResultString = "true";
	}
	ImGui::Text("Knight IsOnGround : %s", ResultString.data());
}

void UDebugWindowGUI::GetFrame()
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	int Result = static_cast<int>(1.0f / DeltaTime);
	ImGui::Text("Frame : %d", Result);
}

void UDebugWindowGUI::ShowLoadingTime()
{
	bool bIsExecute = ATitleGameMode::IsExecute();
	if (true == bIsExecute)
	{
		double Time = double(UEngineCore::EndTime - UEngineCore::StartTime) / CLOCKS_PER_SEC;
		ImGui::Text("Exe Time : %0.4f", Time);
	}
}

void UDebugWindowGUI::ShowPlayLoadingTime()
{
	bool Result = ATitleGameMode::IsPlayStart();
	if (true == Result)
	{
		double Time = double(EndLoadingTime - PlayStartLoadingTime) / CLOCKS_PER_SEC;
		ImGui::Text("Play Loading Time : %0.4f", Time);
	}
}
