
#include "linking/include/imgui/imgui.h"
#include "linking/include/imgui/imgui_impl_glfw.h"
#include "linking/include/imgui/imgui_impl_opengl3.h"
#include "camera.hpp"
#include "worldGeneration.hpp"

class Camera;
class WorldGeneration;
class AdminGui
{
public:
    AdminGui(GLFWwindow *window, Camera *cameraRef, WorldGeneration *worldGeneration);

    void showAdminGui();

    ~AdminGui();

private:
    Camera *cameraRef;
    WorldGeneration *worldGenerationRef;

};