#include "include/adminGui.hpp"
#include <iostream>

AdminGui::AdminGui(GLFWwindow *window, World &worldRef, Shader &shader)
    : worldRef(worldRef),
      shader(shader) {
    std::cout << "admin gui object\n";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    ImGui_ImplOpenGL3_Init("#version 330");
}

void AdminGui::regenerateWorldButton() {

    std::cout << this->worldRef.worldGenerationRef->fractalValue << std::endl;
    this->worldRef.worldMap.clear();
    this->worldRef.update(this->shader);
}
void AdminGui::showAdminGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Test UI");
    ImGui::Text("Hello, world from ImGui!");
    ImGui::Checkbox("Creatif", &this->worldRef.cameraRef->gamemode);

    ImGui::SliderInt("Fractal Octave Value",
                     &this->worldRef.worldGenerationRef->fractalValue, 2, 18);

    ImGui::SliderFloat("Frequency Value",
                       &this->worldRef.worldGenerationRef->frequencyValue,
                       0.01f, 0.05f);

    ImGui::SliderFloat(
        "Fractal Lacunarity Value",
        &this->worldRef.worldGenerationRef->fractalLacunarityValue, 1.5f, 3.0f);

    ImGui::SliderFloat("Fractal Gain Value",
                       &this->worldRef.worldGenerationRef->fractalGainValue,
                       0.3f, 0.8f);

    ImGui::SliderFloat("Amplitude Value",
                       &this->worldRef.worldGenerationRef->amplitudeValue, 2.0f,
                       50.0f);

    if (ImGui::Button("Regenerate World")) {
        regenerateWorldButton();
    }
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
