#include "include/adminGui.hpp"
#include <chrono>
#include <iostream>
int fps = 0;
void AdminGui::calculateFPS() {
    static int frameCount = 0;
    static auto lastTime = std::chrono::high_resolution_clock::now();

    frameCount++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime =
        std::chrono::duration<double, std::milli>(currentTime - lastTime)
            .count();

    if (elapsedTime > 1000.0) {
        std::cout << "FPS: " << frameCount << std::endl;
        fps = frameCount;
        frameCount = 0;
        lastTime = currentTime;
    }
}
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
    ImGui::Begin("Admin Gui");
    ImGui::Text("Generation Settings");

    ImGui::Text("FPS : %d", fps);
    ImGui::Checkbox("Survival", &this->worldRef.cameraRef->gamemode);
    ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f",
                this->worldRef.cameraRef->cameraPos.x,
                this->worldRef.cameraRef->cameraPos.y,
                this->worldRef.cameraRef->cameraPos.z);
    ImGui::Text("Seed : %d", this->worldRef.worldGenerationRef->seed);
    if (ImGui::Button("Generate Seed")) {
        this->worldRef.worldGenerationRef->generateSeed();
    }

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
