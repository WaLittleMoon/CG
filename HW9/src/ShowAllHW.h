//
//  ShowAllHW.h
//  OpenGLTest
//
//  Created by Miss_Jian on 2019/3/23.
//  Copyright © 2019 Rx. All rights reserved.
//

#ifndef ShowAllHW_h
#define ShowAllHW_h
#include <iostream>

#include "Triangle.h"
#include "Bresenham.hpp"
#include "Transform.hpp"
#include "Projection_and_View.hpp"
#include "Light.hpp"
#include "ShadowingMapping.hpp"
#include "BezierCurve.hpp"

#include "Camera.hpp"

void showHW2(bool hw,Shader & ourShader){
    if(hw){
        ImGui::Text("Hello, HW2");
        // 渲染输出图形
        displaySimpleTriange(ourShader);                // basic 1：简单三角形
        displayColorfulTriange(ourShader);              // basic 2：红绿蓝三角形
        displayImGUITriange(ourShader);  // basic 3：颜色可选三角形
        displayPoint(ourShader);                        // bonus 1：点
        displayLine(ourShader);                         // bonus 1：线
        displaySeveralTriangle(ourShader);              // bonus 2：多个三角形
    }
}
void showHW3(bool hw,Shader & ourShader){
    if(hw){
        ImGui::Text("Hello, HW3");
        
        static Bresenham bresenham;
        static ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        
        static int radius = 50;
        static int Rx = 0, Ry = 0;
        
        static int chooseItem = 0;
        static bool padding = false;
        
        // 三角形顶点
        static int xPos[]={-50,0,50};
        static int yPos[]={-50,50,-50};

        static int precision = 100;
        
        ImGui::ColorEdit3("Setting", (float*)&color);
        if(chooseItem==0){
            bresenham.drawTriangle(ourShader, color, xPos, yPos);
            if(padding)
                bresenham.paddingTriangle(ourShader, color, xPos, yPos);
        }
        else{
            bresenham.drawCircle(ourShader, color, Rx, Ry, radius);
            if(padding)
                bresenham.paddingCircle(ourShader, color, Rx, Ry, radius);
        }
        
        ImGui::Text("Choose triangle or circle");
        
        ImGui::RadioButton("triangle", &chooseItem, 0);
        ImGui::RadioButton("circle", &chooseItem, 1);
        ImGui::Checkbox("padding", &padding);

        // 三角形imgui
        if (chooseItem == 0) {
            ImGui::PushItemWidth(50);
            
            ImGui::SliderInt("x0", &xPos[0], -precision, precision);
            ImGui::SameLine();
            ImGui::SliderInt("x1", &xPos[1], -precision, precision);
            ImGui::SameLine();
            ImGui::SliderInt("x2", &xPos[2], -precision, precision);

            ImGui::SliderInt("y0", &yPos[0], -precision, precision);
            ImGui::SameLine();
            ImGui::SliderInt("y1", &yPos[1], -precision, precision);
            ImGui::SameLine();
            ImGui::SliderInt("y2", &yPos[2], -precision, precision);
        }
        
        // 圆imgui
        if (chooseItem == 1) {
            ImGui::PushItemWidth(80);
            ImGui::SliderInt("Rx", &Rx, -precision, precision);
            ImGui::SameLine();
            ImGui::SliderInt("Ry", &Ry, -precision, precision);
            
            ImGui::SliderInt("radius", &radius, 0, 100);
            
        }
    }
}
void showHW4(bool hw,Shader & ourShader,int width, int height){
    if(hw){
        ImGui::Text("Hello, HW4");
        
        static Transform transform;
        
        static int chooseItem = 0;
        static bool depth = true;
        static float translation[]={0.0,0.0,0.0};
        static float degree[]={1.0,0.0,1.0};
        static float scale[]={1.0,1.0,1.0};
        
        transform.Translation(translation);
        transform.Rotation(degree);
        transform.Scaling(scale);
        transform.render(ourShader,depth,width,height,chooseItem);
        
        ImGui::Text("Transform");
        
        ImGui::RadioButton("Translation", &chooseItem, 0);  // 平移
        ImGui::RadioButton("Rotation", &chooseItem, 1);     // 旋转
        ImGui::RadioButton("Scaling", &chooseItem, 2);      // 缩放
        ImGui::RadioButton("Bonus", &chooseItem, 3);        // bonus

        ImGui::Checkbox("Depth", &depth);
        
        
        if (chooseItem == 0) {
            ImGui::PushItemWidth(100);
            
            ImGui::SliderFloat("x", &translation[0], -0.5, 0.5);
            ImGui::SameLine();
            ImGui::SliderFloat("y", &translation[1], -0.5, 0.5);
            ImGui::SameLine();
            ImGui::SliderFloat("z", &translation[2], -0.5, 0.5);
        }
        else if (chooseItem == 1||chooseItem == 3) {
            ImGui::PushItemWidth(100);
            
            ImGui::SliderFloat("x", &degree[0], -1.0, 1.0);
            ImGui::SameLine();
            ImGui::SliderFloat("y", &degree[1], -1.0, 1.0);
            ImGui::SameLine();
            ImGui::SliderFloat("z", &degree[2], -1.0, 1.0);
        }
        else if (chooseItem == 2){
            ImGui::PushItemWidth(100);
            
            ImGui::SliderFloat("x", &scale[0], 0, 3.0);
            ImGui::SameLine();
            ImGui::SliderFloat("y", &scale[1], 0, 3.0);
            ImGui::SameLine();
            ImGui::SliderFloat("z", &scale[2], 0, 3.0);
        }
        
    }
}

void showHW5(bool hw,Shader & ourShader,const unsigned int src_height, const unsigned int src_width,glm::mat4 cameraView,float fov){
    if(hw){
        ImGui::Text("Hello, HW5");
        
        static Projection_and_View proj_and_view = Projection_and_View(src_height,src_width);
        
        static int flag = 0;
        static int chooseItem = 0;
        static bool depth = true;
        static float proj_trans[]={-1.5, 0.5, -1.5};
        static float view_trans[]={0.0, 0.0, 0.0};
        static float ortho[]={-4.0,4.0,-4.0,4.0,0.1,30.0};
        static float persp[]={45.0,1.0,3.0,30.0};
        
        proj_and_view.Translation(flag==0?proj_trans:view_trans);
        if(chooseItem == 1){
            proj_and_view.OrthoProjection(ortho);
        }
        else{
            proj_and_view.PerspProjection(persp);
        }
            
        proj_and_view.render(ourShader,depth,chooseItem,cameraView, fov);

        ImGui::RadioButton("Projection", &flag, 0);
        ImGui::RadioButton("View", &flag, 1);
        ImGui::RadioButton("Camera", &flag, 2);
        if(flag==0){
            ImGui::Text("Hello Projection");
            
            ImGui::RadioButton("Translation", &chooseItem, 0);
            ImGui::RadioButton("Orthographic Projection", &chooseItem, 1);
            ImGui::RadioButton("perspective projection", &chooseItem, 2);
            
            ImGui::Checkbox("Depth", &depth);
            
            
            if (chooseItem == 0) {
                ImGui::PushItemWidth(100);
                
                ImGui::SliderFloat("x", &proj_trans[0], -10, 10);
                ImGui::SameLine();
                ImGui::SliderFloat("y", &proj_trans[1], -10, 10);
                ImGui::SameLine();
                ImGui::SliderFloat("z", &proj_trans[2], -15, 15);
            }
            else if (chooseItem == 1) {
                ImGui::PushItemWidth(100);
                
                ImGui::SliderFloat("left", &ortho[0], -4.0, 4.0);
                ImGui::SameLine();
                ImGui::SliderFloat("right", &ortho[1], -4.0, 4.0);
                
                ImGui::SliderFloat("bottom", &ortho[2], -4.0, 4.0);
                ImGui::SameLine();
                ImGui::SliderFloat("top", &ortho[3], -4.0, 4.0);
                
                ImGui::SliderFloat("near plane", &ortho[4], -5.0, 5.0);
                ImGui::SameLine();
                ImGui::SliderFloat("far plane", &ortho[5], 0.1, 20.0);
            }
            else if (chooseItem == 2){
                ImGui::PushItemWidth(100);
                
                ImGui::SliderFloat("fov", &persp[0], 0.0, 90.0);
                ImGui::SameLine();
                ImGui::SliderFloat("wh_rate", &persp[1], 0.1, 10.0);
                
                ImGui::SliderFloat("near", &persp[2], -5.0, 6.0);
                ImGui::SameLine();
                ImGui::SliderFloat("far", &persp[3], 0.1, 50.0);
            }
        }
        else if(flag==1){
            ImGui::Text("Hello View");
            
            ImGui::RadioButton("rotate", &chooseItem, 3);

            ImGui::Checkbox("Depth", &depth);
        }else{
            ImGui::Text("Hello Camera");
            
            ImGui::RadioButton("rotate", &chooseItem, 4);
            
            ImGui::Checkbox("Depth", &depth);
        }
        
        
    }
}

void showHW6(bool hw,Shader & lampShader, Shader & phongShader, Shader & gouraudShader, int width, int height,Camera camera, float fov){
    if(hw){
        ImGui::Text("Hello, HW6");
        
        static Light light(height, width);
        
        static int chooseItem = 0;
        static bool depth = true;
        static bool LampRotate = false;
        
        static ImVec4 lampColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        static glm::vec3 lampPos = glm::vec3(0.0f, 0.0f, 1.0f);
        
        static float ambientStrength = 0.1;
        static float diffuseStrength = 0.5;
        static float specularStrength = 0.5;
        static int shininess = 32;
        
        ImGui::Text("Transform");
        
        ImGui::RadioButton("Phong Shading", &chooseItem, 0);
        ImGui::RadioButton("Gouraud Shading", &chooseItem, 1);
        
        ImGui::Checkbox("Depth", &depth);
        ImGui::Checkbox("LampRotate", &LampRotate);
        
        ImGui::ColorEdit3("lampColor", (float*)&lampColor);
        
        ImGui::PushItemWidth(100);
        ImGui::SliderFloat("lampPos.x", &lampPos[0], -2.0f, 2.0f);
        ImGui::SliderFloat("lampPos.y", &lampPos[1], -2.0f, 2.0f);
        ImGui::SliderFloat("lampPos.z", &lampPos[2], -5.0f, 5.0f);
        
        ImGui::PushItemWidth(200);
        ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 2.0f);
        ImGui::SliderInt("shininess", &shininess, 0, 256);
        
        light.setArgs(lampColor, ambientStrength, diffuseStrength, specularStrength, shininess);
        
        if(LampRotate){
            lampPos[0] = sin(glfwGetTime()) * 1.0f;
            lampPos[1] = cos(glfwGetTime()) * 1.0f;
            lampPos[2] = sin(glfwGetTime()) * cos(glfwGetTime()) * 2.0f;
        }
        light.updateLampPos(lampPos);
        
        if (chooseItem == 0) {
            light.render(lampShader, phongShader, depth, camera, fov);
            
        }
        else if (chooseItem == 1) {
            light.render(lampShader, gouraudShader, depth, camera, fov);
            
        }
        
    }
}


void showHW7(bool hw,Shader & shader, Shader & simpleDepthShader, int height, int width,Camera camera){
    if(hw){
        ImGui::Text("Hello, HW7");
        static ShadowingMapping shadowmap(shader, height, width);
        
        static int chooseItem = 0;
        static bool shadows = true;
        static bool rotate = false;
        
        static ImVec4 lampColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        static float fov = 45.0f;
        
        static glm::vec3 lightPos=glm::vec3(-2.0f, 4.0f, -1.0f);
        
        ImGui::RadioButton("ortho", &chooseItem, 0);
        ImGui::RadioButton("perspective", &chooseItem, 1);
        
        ImGui::Checkbox("Shadows", &shadows);
        ImGui::Checkbox("LightRotate", &rotate);
        if(chooseItem ==1){
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("Presp fov", &fov, -90.0f, 90.0f);
        }
        
        ImGui::Text("Light Direction");

        ImGui::PushItemWidth(50);
        ImGui::SliderFloat("x", &lightPos[0], -10.0f, 10.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("y", &lightPos[1], -10.0f, 10.0f);
        ImGui::SameLine();
        ImGui::SliderFloat("z", &lightPos[2], -10.0f, 10.0f);
        if(rotate){
            lightPos.x = sin(glfwGetTime()) * 3.0f;
            lightPos.z = cos(glfwGetTime()) * 2.0f;
            lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;
        }
        shadowmap.updateLightDirection(lightPos);
        shadowmap.render(shader, simpleDepthShader, camera, shadows, fov, chooseItem);
                
    }
}
void showHW9(bool hw,Shader & ourShader,int width, int height, vector<glm::vec2> BezierPos,bool change, bool isRun){
    if(hw){
        ImGui::Text("Hello, HW9");
        static BezierCurve bezier(height,width);
        if(change)
            bezier.updateVertices(ourShader,BezierPos);
        if(isRun)
            bezier.render(ourShader);
    }
}
#endif /* ShowAllHW_h */
