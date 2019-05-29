#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "ShowAllHW.h"
#include "Camera.hpp"

#include <vector>
#include <math.h>
using namespace std;
//窗口回调函数，窗口大小被调整的时候被调用。
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
// 输入控制，ESC键关闭窗口
void processInput(GLFWwindow *window);
// 初始化ImGUI界面
void initialImGUI(GLFWwindow* window);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 鼠标点击
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX =  WINDOW_WIDTH / 2.0;
float lastY =  WINDOW_HEIGHT / 2.0;
float fov   =  45.0f;

bool is_mouse_enable = true;

Camera camera=Camera();

vector<glm::vec2> BezierPos;
glm::vec2 CursorPos;
const unsigned int POS_MAX_SIZE = 15;

float lastClick=glfwGetTime();
bool change=false;
bool isRun=false;
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
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, scroll_callback);
    
   // glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // glad: 加载OpenGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);
    initialImGUI(window);
    
    
    Shader  ourShader("resources/shaderSource/shader.vs", "resources/shaderSource/shader.fs");
    Shader  ourShader4("resources/shaderSource/HW4shder.vs", "resources/shaderSource/HW4shder.fs");
    Shader  ourShader5("resources/shaderSource/HW5shader.vs", "resources/shaderSource/HW5shader.fs");
    Shader  phongShader("resources/shaderSource/phong_shading.vs", "resources/shaderSource/phong_shading.fs");
    Shader  gouraudShader("resources/shaderSource/gouraud_shading.vs", "resources/shaderSource/gouraud_shading.fs");
    Shader  lampShader("resources/shaderSource/lamp.vs", "resources/shaderSource/lamp.fs");
    //HW7 Shader
    Shader shader_7("resources/shaderSource/shadow_mapping.vs", "resources/shaderSource/shadow_mapping.fs");
    Shader simpleDepthShader_7("resources/shaderSource/shadow_mapping_depth.vs", "resources/shaderSource/shadow_mapping_depth.fs");
   // Shader debugDepthQuad_7("resources/shaderSource/debug_quad.vs", "resources/shaderSource/debug_quad.fs");
    
    Shader  ourShader9("resources/shaderSource/BezierShader.vs", "resources/shaderSource/BezierShader.fs");
    
    //    Bresenham bresenham;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // 输入控制
        processInput(window);
        // 渲染
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        static bool isHW2 = false;
        static bool isHW3 = false;
        static bool isHW4 = false;
        static bool isHW5 = false;
        static bool isHW6 = false;
        static bool isHW7 = false;
        static bool isHW9 = false;
        
        ImGui_ImplGlfwGL3_NewFrame();
        
        bool ImGui = true;
        ImGui::Begin("CG", &ImGui, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Change"))
            {
                is_mouse_enable=true;
                if(ImGui::MenuItem("HW2", NULL, &isHW2))    {isHW3=false;isHW4=false;isHW5=false;isHW6=false;isHW7=false;isHW9=false;}
                if(ImGui::MenuItem("HW3", NULL, &isHW3))    {isHW2=false;isHW4=false;isHW5=false;isHW6=false;isHW7=false;isHW9=false;}
                if(ImGui::MenuItem("HW4", NULL, &isHW4))    {isHW2=false;isHW3=false;isHW5=false;isHW6=false;isHW7=false;isHW9=false;}
                if(ImGui::MenuItem("HW5", NULL, &isHW5))    {isHW2=false;isHW3=false;isHW4=false;isHW6=false;isHW7=false;isHW9=false;}
                if(ImGui::MenuItem("HW6", NULL, &isHW6))    {isHW2=false;isHW3=false;isHW4=false;isHW5=false;isHW7=false;isHW9=false;}
                if(ImGui::MenuItem("HW7", NULL, &isHW7))    {isHW2=false;isHW3=false;isHW4=false;isHW5=false;isHW6=false;isHW9=false;}
                if(ImGui::MenuItem("HW9", NULL, &isHW9))    {isHW2=false;isHW3=false;isHW4=false;isHW5=false;isHW6=false;isHW7=false;}
                
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        showHW2(isHW2,ourShader);
        
        showHW3(isHW3,ourShader);
        
        showHW4(isHW4,ourShader4, WINDOW_WIDTH, WINDOW_HEIGHT);
        
        showHW5(isHW5,ourShader5, WINDOW_HEIGHT,WINDOW_WIDTH,camera.GetViewMatrix(),fov);
        
        showHW6(isHW6, lampShader, phongShader,gouraudShader, WINDOW_WIDTH, WINDOW_HEIGHT, camera, fov);
        
        showHW7(isHW7, shader_7, simpleDepthShader_7,  WINDOW_HEIGHT, WINDOW_WIDTH, camera);
        
        showHW9(isHW9,ourShader9, WINDOW_WIDTH, WINDOW_HEIGHT, BezierPos,change,isRun);
        
        
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
    
    change=false;
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        is_mouse_enable = !is_mouse_enable;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        isRun = !isRun;
}
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    CursorPos.x = (xpos - WINDOW_WIDTH/2)/(WINDOW_WIDTH/2);
    CursorPos.y = (WINDOW_HEIGHT/2 - ypos)/(WINDOW_HEIGHT/2);
    
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
    camera.ProcessMouseScroll(yoffset);
}


void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    change=true;
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        lastClick = glfwGetTime();
        cout<<BezierPos.size()<<endl;
        if(BezierPos.size() < POS_MAX_SIZE) {
            BezierPos.push_back(CursorPos);
        }
    }
    if(glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        for(auto it = BezierPos.begin(); it!= BezierPos.end() ; it++) {
            if(pow(it->x - CursorPos.x, 2) + pow(it->y - CursorPos.y, 2) <= 0.01) {
                BezierPos.erase(it);
                break;
            }
        }
    }
}
