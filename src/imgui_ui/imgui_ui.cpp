#include "imgui_ui.hpp"

void ImguiUI::init(SDL_Window* window, VulkanContext *context, VulkanRenderPass *renderPass, VkDescriptorPool *imguiDescriptorPool) {
    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Bind ImGui to SDL2
    ImGui_ImplSDL2_InitForVulkan(window);

    // Bind ImGui to Vulkan
    ImGui_ImplVulkan_InitInfo imguiInitInfo = {};
    imguiInitInfo.Instance = context->instance;
    imguiInitInfo.PhysicalDevice = context->physicalDevice;
    imguiInitInfo.Device = context->device;
    imguiInitInfo.QueueFamily = context->graphicsQueue.familyIndex;
    imguiInitInfo.Queue = context->graphicsQueue.queue;
    imguiInitInfo.PipelineCache = VK_NULL_HANDLE;
    imguiInitInfo.DescriptorPool = *imguiDescriptorPool;
    imguiInitInfo.Allocator = nullptr;
    imguiInitInfo.MinImageCount = 2;
    imguiInitInfo.ImageCount = 2;
    imguiInitInfo.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&imguiInitInfo, renderPass->renderPass);

}

void ImguiUI::loadFonts(VulkanContext* context, VkFence* copyFence, VulkanCommandBuffer* copyCommandBuffer) {

    VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VKA(vkBeginCommandBuffer(copyCommandBuffer->commandBuffer, &beginInfo));
    ImGui_ImplVulkan_CreateFontsTexture(copyCommandBuffer->commandBuffer);
    VKA(vkEndCommandBuffer(copyCommandBuffer->commandBuffer));

    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &copyCommandBuffer->commandBuffer;
    VKA(vkQueueSubmit(context->graphicsQueue.queue, 1, &submitInfo, *copyFence));
    VKA(vkWaitForFences(context->device, 1, copyFence, VK_TRUE, UINT64_MAX));
    vkResetFences(context->device, 1, copyFence);

    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void ImguiUI::loadStyle() {
    // Future Dark style by rewrking from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(20.0f, 20.0f);
	style.WindowRounding = 20.0f;
	style.WindowBorderSize = 5.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(6.0f, 6.0f);
	style.FrameRounding = 10.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(22.0f, 20.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
	style.CellPadding = ImVec2(12.0f, 6.0f);
	style.IndentSpacing = 20.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 12.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);


    // Standard Fonts
    ImGuiIO& io = ImGui::GetIO();

    ImFontAtlas* fontAtlas = io.Fonts;
    ImFont* std_font = fontAtlas->AddFontFromFileTTF(("C:\\Windows\\Fonts\\micross.ttf"), 20);
    ImFont* fancy_font = fontAtlas->AddFontFromFileTTF(("C:\\Windows\\Fonts\\OCRAEXT.TTF"), 30);
    ImFont* big_timer_font = fontAtlas->AddFontFromFileTTF(("C:\\Windows\\Fonts\\impact.ttf"), 60);
    ImFont* small_timer_font = fontAtlas->AddFontFromFileTTF(("C:\\Windows\\Fonts\\impact.ttf"), 30);
    ImFont* fancy_font_huge = fontAtlas->AddFontFromFileTTF(("C:\\Windows\\Fonts\\OCRAEXT.TTF"), 60);

    fontAtlas->Build();
}

void ImguiUI::newFrame() {
    ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void ImguiUI::render(VkCommandBuffer* commandBuffer) {
    static bool showDemoWindow = false;
	if(showDemoWindow) {
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
    
    mainGame();
    //mainMenue();
    //pauseMenue();
    //errorDialog();
    //settingsMenue();

    pythonError();

    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();
    ImGui_ImplVulkan_RenderDrawData(drawData, *commandBuffer);
}


void ImguiUI::mainGame() {

    ImGuiIO& io = ImGui::GetIO();
    // Set window flags
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                                   ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | 
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                   ImGuiWindowFlags_NoDecoration;

    // Set window size and position
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    // Begin the fullscreen ImGui window
    ImGui::Begin("Fullscreen Window", nullptr, windowFlags);

    // Disable Scrolling and Padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    // Draw webcam image
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::Image(webcamImageGUI, io.DisplaySize);

    // Draw timer
    ImGui::SetCursorPos(ImVec2(20, 20));
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.2f, 1, 1));
    ImGui::Text("Time:");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(20, 40));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    ImGui::Text("00:00");
    ImGui::PopFont();

    // Draw score
    ImGui::SetCursorPos(ImVec2(20, 90));
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.2f, 1, 1));
    ImGui::Text("Score:");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(20, 110));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    ImGui::Text("000000");
    ImGui::PopFont();

    // Draw gameover text
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[4]);
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5);
    ImVec2 text_size = ImGui::CalcTextSize("Game Over");
    ImGui::SetCursorPos(ImVec2(center.x - text_size.x * 0.5, center.y - text_size.y * 0.5));
    ImGui::Text("Game Over");
    ImGui::PopFont();

    // Draw Final Score
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
    center = ImVec2(io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5);
    text_size = ImGui::CalcTextSize("Final Score: 000000");
    ImGui::SetCursorPos(ImVec2(center.x - text_size.x * 0.5, center.y - text_size.y * 0.5 + 50));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.2f, 1, 1));
    ImGui::Text("Final Score: 000000");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // Draw Play Again and Mian Menue buttons
    ImGui::SetCursorPos(ImVec2(center.x - 100, center.y + 80));
    if (ImGui::Button("Play Again", ImVec2(200, 50))) {
        // Start new game
    }

    ImGui::SetCursorPos(ImVec2(center.x - 100, center.y + 150));
    if (ImGui::Button("Main Menue", ImVec2(200, 50))) {
        // Start new game
    }

    // Draw Pause/Resume button
    ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - 120, 20));
    if (ImGui::Button("Pause", ImVec2(100, 50))) {
        // Pause/Resume game
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw Crosshair
    DrawCrosshair(drawList, io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5, 20, ImColor(255, 20, 147));	
    DrawCrosshair(drawList, io.DisplaySize.x * 0.5 + 100, io.DisplaySize.y * 0.5 + 200, 10, ImColor(100, 50, 200));


    // Enable Scrolling and Padding
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    // End the ImGui window
    ImGui::End();
}

void ImguiUI::DrawCrosshair(ImDrawList* drawList, float x, float y, float size, ImColor color) {
    drawList->AddLine(ImVec2(x - size, y), ImVec2(x + size, y), color);
    drawList->AddLine(ImVec2(x, y - size), ImVec2(x, y + size), color);

    drawList->AddCircle(ImVec2(x, y), size, color);
}

void ImguiUI::errorDialog() {
    ImGuiIO& io = ImGui::GetIO();

    // Set window size and position
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5);
    ImVec2 windowSize = ImVec2(io.DisplaySize.x * 0.8, io.DisplaySize.y * 0.3);
    ImVec2 windowPos = ImVec2(center.x - windowSize.x * 0.5, center.y - windowSize.y * 0.5);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("ErrorDialog", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Draw error message
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::Text("Error Message");
    ImGui::PopFont();

    // Draw OK button
    if (ImGui::Button("OK", ImVec2(100, 50))) {
        // Close error dialog
    }

    // End the ImGui window
    ImGui::End();
}

void ImguiUI::pythonError() {
    ImGuiIO& io = ImGui::GetIO();

    // Set window size and position
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5);
    ImVec2 windowSize = ImVec2(io.DisplaySize.x * 0.8, io.DisplaySize.y * 0.3);
    ImVec2 windowPos = ImVec2(center.x - windowSize.x * 0.5, center.y - windowSize.y * 0.5);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("Python Error", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Draw error message
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
    ImGui::Text("The Game did not exit properly last time. This caused python instances to stay alive. Please close all python instances.");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // Draw buttons
    if (ImGui::Button("OK, I closed all python instances")) {
        // Close error dialog
    }
    ImGui::SameLine();
    if (ImGui::Button("KILL ALL PYTHON INSTANCES")) {
        // Close error dialog
    }

    // End the ImGui window
    ImGui::End();
}

void ImguiUI::settingsMenue() {
    ImGui::Begin("Settings");

    if (ImGui::BeginTabBar("SettingsTabBar"))
    {
        if (ImGui::BeginTabItem("Global"))
        {
            ImGui::Text("Difficulty");
            const char* difficulty[] = { "Easy", "Medium", "Hard" };
            static int selectedDifficulty = 0;
            ImGui::Combo("Difficulty", &selectedDifficulty, difficulty, IM_ARRAYSIZE(difficulty));

            ImGui::Text("Sensor");
            const char* sensor[] = { "Shimmer", "EEG", "Dummy"};
            static int selectedSensor = 0;
            ImGui::Combo("Sensor", &selectedSensor, sensor, IM_ARRAYSIZE(sensor));

            ImGui::Text("Time");
            const char* time[] = { "30s", "60s", "90s" };
            static int selectedTime = 0;
            ImGui::Combo("Time", &selectedTime, time, IM_ARRAYSIZE(time));

            ImGui::Text("Testing");
            bool virtualMovingHead = false;
            ImGui::Checkbox("Virtual MovingHead", &virtualMovingHead);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Python"))
        {
            ImGui::Text("Python Path");
            char pythonPath[] = "C:\\Python\\Python38\\python.exe";
            ImGui::InputText("PythonPath", pythonPath, IM_ARRAYSIZE(pythonPath));

            ImGui::Text("Test Python Path");
            ImGui::Button("Test", ImVec2(100, 50));

            ImGui::Text("Python Kill Processes");
            ImGui::Button("Kill", ImVec2(100, 50));

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Sensor Schimmer"))
        {
            ImGui::Text("Shimmer Sensor");

            ImGui::Text("COM Port");
            char ShimmerCOMPort[] = "COM1";
            ImGui::InputText("", ShimmerCOMPort, IM_ARRAYSIZE(ShimmerCOMPort));

            ImGui::Text("Polling Rate");
            int ShimmerPollingRate = 20;
            ImGui::SliderInt("", &ShimmerPollingRate, 1, 30);

            ImGui::Button("Connect", ImVec2(100, 50));
            ImGui::SameLine();
            ImGui::Button("Disconnect", ImVec2(100, 50));

            ImGui::Text("Sensor Status");
            char ShimmerStatus[] = "Connected";
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
            ImGui::Text(ShimmerStatus, IM_ARRAYSIZE(ShimmerStatus));
            ImGui::PopStyleColor();

            ImGui::Text("Python Log");
            char pythonLog[] = "Lorem ipsum dolor sit amet, consectetur \n adipiscing elit. Donec euismod, nisl eget ultricies ultrices, \n nunc nisl ultricies nisl, nec\n aliquam nisl nisl vitae";
            ImGui::InputTextMultiline("", pythonLog, IM_ARRAYSIZE(pythonLog), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Sensor EEG"))
        {
            ImGui::Text("EEG");

            ImGui::Text("API UserID");
            char UserID[] = "jhgsldfkfhdgksghfdgskjsk";
            ImGui::InputText("", UserID, IM_ARRAYSIZE(UserID));

            ImGui::Text("API Token");
            char Token[] = "jhgsldfkfhdgksghfdgskjsksddsdsddsdsdsadghadsgssfdghdgjksafdfg";
            ImGui::InputText("", Token, IM_ARRAYSIZE(Token));

            ImGui::Text("Polling Rate");
            int ShimmerPollingRate = 20;
            ImGui::SliderInt("", &ShimmerPollingRate, 1, 30);

            ImGui::Button("Connect", ImVec2(100, 50));
            ImGui::SameLine();
            ImGui::Button("Disconnect", ImVec2(100, 50));

            ImGui::Text("Sensor Status");
            char ShimmerStatus[] = "Connected";
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
            ImGui::Text(ShimmerStatus, IM_ARRAYSIZE(ShimmerStatus));
            ImGui::PopStyleColor();

            ImGui::Text("Python Log");
            char pythonLog[] = "Lorem ipsum dolor sit amet, consectetur \n adipiscing elit. Donec euismod, nisl eget ultricies ultrices, \n nunc nisl ultricies nisl, nec\n aliquam nisl nisl vitae";
            ImGui::InputTextMultiline("", pythonLog, IM_ARRAYSIZE(pythonLog), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Moving Head"))
        {
            ImGui::Text("Moving Head");

            ImGui::Text("StepSize");
            int StepSize = 20;
            ImGui::SliderInt("", &StepSize, 1, 30);

            ImGui::Button("Connect", ImVec2(100, 50));
            ImGui::SameLine();
            ImGui::Button("Disconnect", ImVec2(100, 50));

            ImGui::Text("Sensor Status");
            char ShimmerStatus[] = "Connected";
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
            ImGui::Text(ShimmerStatus, IM_ARRAYSIZE(ShimmerStatus));
            ImGui::PopStyleColor();

            ImGui::Text("Python Log");
            char pythonLog[] = "Lorem ipsum dolor sit amet, consectetur \n adipiscing elit. Donec euismod, nisl eget ultricies ultrices, \n nunc nisl ultricies nisl, nec\n aliquam nisl nisl vitae";
            ImGui::InputTextMultiline("", pythonLog, IM_ARRAYSIZE(pythonLog), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_ReadOnly);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Button("Save", ImVec2(100, 50));
    ImGui::SameLine();
    ImGui::Button("Cancel", ImVec2(100, 50));

    ImGui::End();
}

void ImguiUI::pauseMenue() {
    ImGuiIO& io = ImGui::GetIO();

    // Set window size and position
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5);
    ImVec2 windowSize = ImVec2(io.DisplaySize.x * 0.4, io.DisplaySize.y * 0.3);
    ImVec2 windowPos = ImVec2(center.x - windowSize.x * 0.5, center.y - windowSize.y * 0.5);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("PauseMenu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Draw the game title with the fancy font
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.2f, 0.2f, 1));
    ImGui::Text("Pause");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // Draw the buttons
    ImGui::SetCursorPosX(windowSize.x * 0.1);
    if (ImGui::Button("Resume", ImVec2(windowSize.x * 0.8, 0))) {
        // Resume the game
    }
    ImGui::SetCursorPosX(windowSize.x * 0.1);
    if (ImGui::Button("Main Menu", ImVec2(windowSize.x * 0.8, 0))) {
        // Go back to the main menu
    }

    ImGui::End();
}

void ImguiUI::mainMenue() {
    ImGuiIO& io = ImGui::GetIO();

    // Set window size and position
    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5, io.DisplaySize.y * 0.5);
    ImVec2 windowSize = ImVec2(io.DisplaySize.x * 0.4, io.DisplaySize.y * 0.6);
    ImVec2 windowPos = ImVec2(center.x - windowSize.x * 0.5, center.y - windowSize.y * 0.5);
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("MainMenu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    // Draw the game title with the fancy font
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0.2f, 1, 1));
    ImGui::Text("Game Title");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    // Draw a separator line
    ImGui::Separator();

    // Draw the menu buttons with big size and spacing
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 20));


    ImGui::SetCursorPosX(windowSize.x * 0.1);
    if (ImGui::Button("Start", ImVec2(windowSize.x * 0.8, 0))) {
    }

    ImGui::SetCursorPosX(windowSize.x * 0.1);
    if (ImGui::Button("Settings", ImVec2(windowSize.x * 0.8, 0))) {
    }

    ImGui::SetCursorPosX(windowSize.x * 0.1);
    if (ImGui::Button("Exit", ImVec2(windowSize.x * 0.8, 0))) {
    }

    ImGui::PopStyleVar(2);

    // End the window
    ImGui::End();
}

void ImguiUI::destroy() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}