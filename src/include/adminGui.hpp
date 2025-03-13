
#include "../linking/include/imgui/imgui.h"
#include "../linking/include/imgui/imgui_impl_glfw.h"
#include "../linking/include/imgui/imgui_impl_opengl3.h"
#include "camera.hpp"
#include "worldGeneration.hpp"

class AdminGui {
    public:
        AdminGui(GLFWwindow *window, World &worldRef, Shader &shader);
        void calculateFPS();
        void regenerateWorldButton();
        void showAdminGui();

        ~AdminGui();

    private:
        Shader &shader;
        World &worldRef;
};
