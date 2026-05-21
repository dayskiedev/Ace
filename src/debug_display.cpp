#include "debug_display.h"

bool debug_display::Init(SDL_Window* _window, SDL_Renderer* _renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(_window,_renderer);
    if(!ImGui_ImplSDLRenderer3_Init(_renderer)) {
        return false;
    }

    return true;
}

void debug_display::Update(SDL_Event e, c8_utils& utils, c8_emulator& emulator, bool& paused) {

        ImGui_ImplSDL3_ProcessEvent(&e);

    // update imgui stuff
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        //Demo
        //ImGui::ShowDemoWindow();

        if(_startup) {
            ImGui::Begin("Startup", NULL, ImGuiWindowFlags_NoCollapse);
            ImGui::PushFont(NULL, 30);
            ImGui::TextWrapped("Enter the directory to a rom and select 'Load Rom' to begin!");
            ImGui::PopFont();
            ImGui::End();
        }      

        //custom gui stuff
        ImGui::Begin("Chip8 Information", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::PushFont(NULL, 20);
        //ImGui::Text(("Current Rom: " + rom).c_str());
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), ("Program Counter: " + std::to_string(emulator.GetPC())).c_str());
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), ("Index Register: " + std::to_string(emulator.GetIR())).c_str());
        //ImGui::Text(("Tick Count: " + std::to_string(tickCount)).c_str());
        //ImGui::Text(("Instruction Count: " + std::to_string(insTickCount)).c_str());
        ImGui::PopFont();
        ImGui::End();

        ImGui::Begin("Address Stack", NULL, ImGuiWindowFlags_NoCollapse);
        std::stack<uint16_t> stack_addr_ref = emulator.GetAS();
        while (!stack_addr_ref.empty())
        {
            ImGui::Text(utils.IntToHex(stack_addr_ref.top()).c_str());
            stack_addr_ref.pop();
        }
        ImGui::End();
        

        ImGui::Begin("ACE Information", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::PushFont(NULL, 18);
        //ImGui::Text("Window Resolution: %dX%d", SCREEN_WIDTH, SCREEN_HEIGHT);
        //ImGui::Text("Emulator Resolution: %dX%d", EMU_WINDOW_WIDTH, EMU_WINDOW_HEIGHT);
        ImGui::PopFont();
        ImGui::End();


        ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoCollapse);
        if(ImGui::Button("Load Rom")) {
            if(!emulator.Startup(inputRom)) {
                loadError = true;
                //emulator.Startup(curRom);
            } else {
                curRom = inputRom;
                loadError = false;
                // we only need this once...
                _startup = false;
            }
        
        }
        ImGui::SameLine();
        ImGui::InputText("##", &inputRom);

        if(loadError) {
            ImGui::Text("Error loading Rom!");
        }

        if(ImGui::Button("Reload Rom")) {
            if(!emulator.Startup(curRom)) {
                loadError = true;
            } else {
                loadError = false;
                paused = false;
            }
        }
        ImGui::Checkbox("Pause Emulator", &paused);
        ImGui::End();

        int indexCount = 0;
        
        ImGui::Begin("Registers", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginTable("Registers", 4);    
        for(int row = 0; row < 4; ++row) {
            ImGui::TableNextRow();
            for(int col = 0; col < 4; ++col) {
                ImGui::PushFont(NULL, 40);
                ImGui::TableSetColumnIndex(col);
                // inttohex adds 0 for ease of reasing, but we dont want that here, so we chop it out
            
                ImGui::Text("V%s: %d", (utils.IntToHex(indexCount).substr(1,1)).c_str(), emulator.GetRV(indexCount));
                ImGui::PopFont();
                indexCount++;
            }
        }
        ImGui::EndTable();
        ImGui::End();

        // 0 1 2 3
        // 4 5 6

        ImGui::Begin("Rom Memory", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginTable("Memory", 4);    
        // for(int row = 0; row < 4; ++row) {
        //     ImGui::TableNextRow();
        //     for(int col = 0; col < 4; ++col) {
        //         ImGui::TableSetColumnIndex(col);
        //         ImGui::Text(std::to_string(row + (col * 4) + 512).c_str());
        //     }
        // }

        indexCount = 0;

        // +1 is to account for offset from romsize being 0 indexed.
        for(int row = 0; row < (emulator.GetRomSize() / 4) +1; ++row) {
            ImGui::TableNextRow();
            for(int col = 0; col < 4; ++col) {
                ImGui::TableSetColumnIndex(col);

                int curMemAdr = indexCount + emulator.GetStartAddr();
                std::string memAsHex = utils.IntToHex(emulator.GetMemoryValue(curMemAdr));

                // check if current memory index is what the Index Register in our emulator is selecting
                if(emulator.GetIR() == curMemAdr) {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));
                    if(col == 3 ) { continue;}
                    ImGui::TableSetColumnIndex(col+1);
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)));
                    //ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), memAsHex.c_str());
                }
                else if(curMemAdr == emulator.GetPC()) {
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));
                    if(col == 3 ) { continue;}
                    ImGui::TableSetColumnIndex(col+1);
                    ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 1.0f, 1.0f)));
                    //ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), memAsHex.c_str());
                } else {
                    ImGui::Text(memAsHex.c_str());
                }
                indexCount++;
            }
        }
        ImGui::EndTable();
        ImGui::End();
}

void debug_display::Render(SDL_Renderer* renderer) {
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void debug_display::Close() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}