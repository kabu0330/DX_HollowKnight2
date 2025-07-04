#include "PreCompile.h"
#include "EngineGUI.h"
#include "EngineCore/EngineCore.h"
#include <EnginePlatform/EngineWindow.h>
#include "EngineGuiWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

UEngineGUI::UEngineGUI()
{
}

UEngineGUI::~UEngineGUI()
{
}

void UEngineGUI::Init()
{

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(UEngineCore::GetMainWindow().GetWindowHandle());
    ImGui_ImplDX11_Init(UEngineCore::GetDevice().GetDevice(), UEngineCore::GetDevice().GetContext());

    // 한글을 사용할수 없는 폰트라 직접 처리한다.

    UEngineDirectory NewDir;
    NewDir.MoveParentToDirectory("EngineResources");
    NewDir.MovePath("Font");
    UEngineFile File = NewDir.GetFile("malgun.ttf");

    File.GetPathToString();
    std::string UTF8Path = UEngineString::AnsiToUTF8(File.GetPathToString());

    io.Fonts->AddFontFromFileTTF(UTF8Path.c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesKorean());

    UEngineWindow::SetCustomProc(
        [=](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            {
                return true;
            }

            return false;
        }
    );
}

void UEngineGUI::Release()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UEngineGUI::StartGUIRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void UEngineGUI::EndGUIRender()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void UEngineGUI::PushGUIWindow(std::shared_ptr<class UEngineGUIWindow> _Window)
{
    _Window->BeginPlay();

    Windows.insert({ _Window->GetName(), _Window });
}

std::shared_ptr<UEngineGUIWindow> UEngineGUI::FindGUIWindow(std::string_view _Text)
{
    std::string UpperName = UEngineString::ToUpper(_Text);

    if (false == Windows.contains(UpperName))
    {
        return nullptr;
    }

    return Windows[UpperName];
}

void UEngineGUI::ActivateAllWindow()
{
    for (std::pair<const std::string, std::shared_ptr<UEngineGUIWindow>>& Window : Windows)
    {
        Window.second->SetActive(true);
    }
}

void UEngineGUI::DeactivateAllWindow()
{
    for (std::pair<const std::string, std::shared_ptr<UEngineGUIWindow>>& Window : Windows)
    {
        Window.second->SetActive(false);
    }
}

void UEngineGUI::GUIRender(ULevel* _Level)
{
    UEngineGUI::StartGUIRender();
    for (std::pair<const std::string, std::shared_ptr<UEngineGUIWindow>>& Window : Windows)
    {
        if (false == Window.second->IsActive())
        {
            continue;
        }

        bool* ActivePtr = &Window.second->GetIsActiveValueRef();
        bool Result = ImGui::Begin(Window.first.c_str(), ActivePtr);

        Window.second->World = _Level;
        Window.second->OnGUI();
        ImGui::End();
    }
    UEngineGUI::EndGUIRender();
}