#include "debugger.h"
#include "defs.h"

#include "imgui/imgui_sdl.h"

Debugger::Debugger(Mmu *pmmu, Cpu *pcpu)
{
    window = SDL_CreateWindow("GasyBoy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH_TEMP, HEIGHT_TEMP, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    /* Initialize ImGUI*/
    ImGui::CreateContext();
    ImGui_ImplSDL2_Init(window);
    ImGuiSDL::Initialize(renderer, WIDTH_TEMP, HEIGHT_TEMP);

    mmu = pmmu;
    stackView = new Stack_viewer(cpu, mmu);
    memview = new Memory_viewer(mmu);
    cpu = pcpu;
    reg_viewer = new Register_viewer(cpu);
    instrViewer = new Instruction_viewer(cpu,mmu);
}

Debugger::~Debugger()
{
    /* Deinitialize ImGUI */
    ImGuiSDL::Deinitialize();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


void Debugger::HandleEvent(SDL_Event event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void Debugger::Render()
{
    /* Begin a new frame */
    ImGui::NewFrame();
    ImGui_ImplSDL2_NewFrame(window);

    instrViewer->Render();
    reg_viewer->Render();
    memview->Render();
    stackView->Render();

    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}

void Debugger::step()
{
    if (cpu->get_cpuState())
            cpu->cpuStep();
    //instrViewer->mouseEvent();
}
