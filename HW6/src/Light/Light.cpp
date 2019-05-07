//
//  Light.cpp
//  OpenGLTest
//
//  Created by Miss_Jian on 2019/5/5.
//  Copyright © 2019 Rx. All rights reserved.
//

#include "Light.hpp"

Light::Light(const unsigned int src_height, const unsigned int src_width): scr_width(src_width), scr_height(src_height){}

void Light::setArgs(ImVec4 lampColor, float ambientStrength, float diffuseStrength, float specularStrength, int shininess){
    this->lampColor = lampColor;
    this->ambientStrength = ambientStrength;
    this->diffuseStrength = diffuseStrength;
    this->specularStrength = specularStrength;
    this->shininess = shininess;
}

void Light::render(Shader &lampShader, Shader &lightingShader, bool depth, Camera camera, float fov){
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("lightColor", lampColor.x, lampColor.y, lampColor.z);
    lightingShader.setVec3("lightPos", lightPos);
    lightingShader.setVec3("viewPos", camera.Position);
    
    lightingShader.setFloat("ambientStrength", ambientStrength);
    lightingShader.setFloat("diffuseStrength", diffuseStrength);
    lightingShader.setFloat("specularStrength", specularStrength);
    lightingShader.setInt("shininess", shininess);

    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)scr_width / (float)scr_width, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    
    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);
    
    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    
    // also draw the lamp object
    lampShader.use();
    lampShader.setMat4("projection", projection);
    lampShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lampShader.setMat4("model", model);
    
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    if (depth) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    
}

void Light::updateLampPos(glm::vec3 lightPos){
    this->lightPos = lightPos;
}
