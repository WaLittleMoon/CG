//
//  BezierCurve.hpp
//  OpenGLTest
//
//  Created by Miss_Jian on 2019/5/27.
//  Copyright © 2019 Rx. All rights reserved.
//

#ifndef BezierCurve_hpp
#define BezierCurve_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"

#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"

#include "Shader.h"
#include <vector>
using namespace std;

class BezierCurve{
public:
    BezierCurve(const int height,const int width);
    void updateVertices(Shader & ourShader, vector<glm::vec2> BezierPos);
    void drawLine(Shader & ourShader, int PosCount);
    void render(Shader & ourShader);
private:
    
    int scr_height;
    int scr_width;
    
    int PointCount = 0;
    int CurveCount;
    
    float tStrength = 0;
    
    float curve[100000];
    
    float vertices[200];
    float tempVertices[200];
    
    float lineVertex[200];
};


#endif /* BezierCurve_hpp */
