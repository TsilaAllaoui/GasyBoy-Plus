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
    cpu = pcpu;
  
    stackView = new Stack_viewer(cpu, mmu);
    memview = new Memory_viewer(mmu);
    reg_viewer = new Register_viewer(cpu);
    instrViewer = new Instruction_viewer(cpu,mmu);
    vram_viewer = new Vram_viewer(mmu);
    vram_viewer->Init();
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

    reg_viewer->Render();
    instrViewer->Render();
    memview->Render();
    stackView->Render(mmu);
    //if (mmu->read_ram(0x8000) == 0xF0)
    if (cpu->get_PC() >= 0xC)
        vram_viewer->Render();

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

void Debugger::step()
{
    if (cpu->get_cpuState() == RUNNING)
        cpu->cpuStep();
    /*else if (cpu->get_cpuState() == RESETED)
    {
        //mmu = new Mmu();
        cpu = new Cpu(mmu);
        stackView = new Stack_viewer(cpu, mmu);
        memview = new Memory_viewer(mmu);
        reg_viewer = new Register_viewer(cpu);
    }*/
    //instrViewer->mouseEvent();
}
