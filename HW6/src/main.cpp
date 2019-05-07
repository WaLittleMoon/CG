#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "ShowAllHW.h"
#include "Camera.hpp"

//窗口回调函数，窗口大小被调整的时候被调用。
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
// 输入控制，ESC键关闭窗口
void processInput(GLFWwindow *window);
// 初始化ImGUI界面
void initialImGUI(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX =  WINDOW_WIDTH / 2.0;
float lastY =  WINDOW_HEIGHT / 2.0;
float fov   =  45.0f;

bool is_mouse_enable = true;

Camera camera=Camera();
int main()
{
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // 创建glfw窗口
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Draw CG HW", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // glad: 加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    initialImGUI(window);
    
    
    Shader  ourShader("resources/shaderSource/shader.vs", "resources/shaderSource/shader.fs");
    Shader  ourShader4("resources/shaderSource/HW4shder.vs", "resources/shaderSource/HW4shder.fs");
    Shader  ourShader5("resources/shaderSource/HW5shader.vs", "resources/shaderSource/HW5shader.fs");
    Shader  phongShader("resources/shaderSource/phong_shading.vs", "resources/shaderSource/phong_shading.fs");
    Shader  gouraudShader("resources/shaderSource/gouraud_shading.vs", "resources/shaderSource/gouraud_shading.fs");
    Shader  lampShader("resources/shaderSource/lamp.vs", "resources/shaderSource/lamp.fs");
    
//    Bresenham bresenham;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // 输入控制
        processInput(window);
        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        static bool isHW2 = false;
        static bool isHW3 = false;
        static bool isHW4 = false;
        static bool isHW5 = false;
        static bool isHW6 = false;

        ImGui_ImplGlfwGL3_NewFrame();
        
        bool ImGui = true;
        ImGui::Begin("CG", &ImGui, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Change"))
            {
                is_mouse_enable=true;
                if(ImGui::MenuItem("HW2", NULL, &isHW2))    {isHW3=false;isHW4=false;isHW5=false;isHW6=false;}
                if(ImGui::MenuItem("HW3", NULL, &isHW3))    {isHW2=false;isHW4=false;isHW5=false;isHW6=false;}
                if(ImGui::MenuItem("HW4", NULL, &isHW4))    {isHW2=false;isHW3=false;isHW5=false;isHW6=false;}
                if(ImGui::MenuItem("HW5", NULL, &isHW5))    {isHW2=false;isHW3=false;isHW4=false;isHW6=false;}
                if(ImGui::MenuItem("HW6", NULL, &isHW6))    {isHW2=false;isHW3=false;isHW4=false;isHW5=false;is_mouse_enable=false;}

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        showHW2(isHW2,ourShader);
        
        showHW3(isHW3,ourShader);
        
        showHW4(isHW4,ourShader4, WINDOW_WIDTH, WINDOW_HEIGHT);
        
        showHW5(isHW5,ourShader5, WINDOW_HEIGHT,WINDOW_WIDTH,camera.GetViewMatrix(),fov);
        
        showHW6(isHW6, lampShader, phongShader,gouraudShader, WINDOW_WIDTH, WINDOW_HEIGHT, camera, fov);
        
        ImGui::End();
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);    // 检查触发事件
        glfwPollEvents();           // 交换颜色缓冲
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void initialImGUI(GLFWwindow* window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsClassic();
    
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 4.0 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBack(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(cameraSpeed);

}
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(is_mouse_enable){
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        
        lastX = xpos;
        lastY = ypos;
        
        camera.rotate(xoffset, yoffset);
    }
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}
