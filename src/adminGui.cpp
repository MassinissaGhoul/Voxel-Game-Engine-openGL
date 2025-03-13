#include "include/adminGui.hpp"
#include <iostream>

AdminGui::AdminGui(GLFWwindow *window, Camera *cameraRef,
                   WorldGeneration *worldGeneration)
    : cameraRef(cameraRef),
      worldGenerationRef(worldGeneration) {
    std::cout << "admin gui object\n";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init("#version 330");
}

void AdminGui::showAdminGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test UI");
    ImGui::Text("Hello, world from ImGui!");
    ImGui::Checkbox("Creatif", &this->cameraRef->gamemode);
    ImGui::SliderInt("Float Slider", &this->worldGenerationRef->fractalValue, 2,
                     10);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

AdminGui::~AdminGui() {
    std::cout << " destruction admin gui object\n";
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
