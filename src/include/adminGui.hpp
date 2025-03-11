
#include "linking/include/imgui/imgui.h"
#include "linking/include/imgui/imgui_impl_glfw.h"
#include "linking/include/imgui/imgui_impl_opengl3.h"
#include "camera.hpp"

class Camera;
class AdminGui
{
public:
    AdminGui(GLFWwindow *window, Camera *cameraRef);

    void showAdminGui();

    ~AdminGui();

private:
    Camera *cameraRef;
};