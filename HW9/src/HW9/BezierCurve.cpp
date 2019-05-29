//
//  BezierCurve.cpp
//  OpenGLTest
//
//  Created by Miss_Jian on 2019/5/27.
//  Copyright © 2019 Rx. All rights reserved.
//

#include "BezierCurve.hpp"

// 阶乘
int Factorial(int n) {
    if (n == 0) return 1;
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}
float getBernstein(int i, int n, float t) {
    return Factorial(n) / (Factorial(i) * Factorial(n - i)) * pow(t, i) * pow(1 - t, n - i);
}
BezierCurve::BezierCurve(const int height,const int width){
    scr_height=height;
    scr_width=width;
}
void BezierCurve::updateVertices(Shader & ourShader,vector<glm::vec2> BezierPos){
    PointCount=BezierPos.size();
    
    for (int i = 0; i < BezierPos.size(); i++) {
        vertices[i * 2] = BezierPos[i].x;
        vertices[i * 2 + 1] = BezierPos[i].y;
    }
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //坐标属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    ourShader.use();
    glBindVertexArray(VAO);
    
    ourShader.setVec3("ourColor", glm::vec3(0.5,0.5,0.5));
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, PointCount);
    
    ourShader.setVec3("ourColor", glm::vec3(0.0,0.5,0.5));
    glLineWidth(5.0f);
    glDrawArrays(GL_LINE_STRIP, 0, PointCount);

    
    CurveCount = 0;
    if(PointCount > 1){
        for (float t = 0; t < 1; t += 0.001) {
            float c_x = 0, c_y = 0;
            for (int i = 0; i < PointCount; i++) {
                float bernstein = getBernstein(i, PointCount-1, t);
                c_x += BezierPos[i].x * bernstein;
                c_y += BezierPos[i].y * bernstein;
            }
            curve[CurveCount * 2] = c_x;
            curve[CurveCount * 2 + 1] = c_y;
            CurveCount++;
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(curve), curve, GL_STATIC_DRAW);
    
    //坐标属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    ourShader.use();
    glBindVertexArray(VAO);
    
    ourShader.setVec3("ourColor", glm::vec3(0.5,0.5,0.5));
    glPointSize(2.0f);
    glDrawArrays(GL_POINTS, 0, CurveCount);

//    glLineWidth(5.0f);
//    glDrawArrays(GL_LINE_STRIP, 0, CurveCount);
}

void BezierCurve::drawLine(Shader & ourShader, int PosCount){

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertex), lineVertex, GL_STATIC_DRAW);
    
    //坐标属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    ourShader.use();
    glBindVertexArray(VAO);
    
    ourShader.setVec3("ourColor", glm::vec3(0.0,0.5,0.5));
    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, PosCount);
}

void BezierCurve::render(Shader &ourShader){
    if (PointCount > 2) {
        
        //shader->SetVec3("drawColor", glm::vec3(1, 1, 1));
        tStrength += 0.005;
        if (tStrength > 1)
            tStrength = 0;
        
        int leftPos = PointCount;
        for (int i = 0; i < PointCount * 2; i++) {
            tempVertices[i] = vertices[i];
        }
        while (leftPos > 1) {
            int newCount = 0;
            
            for (int i = 0; i < leftPos - 1; i++) {
                float px = tempVertices[i*2] * (1 - tStrength) + tempVertices[i*2+2] * tStrength;
                float py =tempVertices[i*2+1] * (1 - tStrength) + tempVertices[i*2+3] * tStrength;
                lineVertex[newCount*2] = px;
                lineVertex[newCount*2+1] = py;
                newCount++;
            }
            drawLine(ourShader, newCount);
            for (int i = 0; i < newCount * 2; i++) {
                tempVertices[i] = lineVertex[i];
            }
            
            leftPos = newCount;
        }
        
    }    
}
