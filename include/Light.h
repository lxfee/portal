#pragma once
#include "Model.h"
#include "Global.h"

struct DirLight {
    DirLight();
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    void transLight(const string &name, Shader* shader);
    void doMovement(unsigned char* KEYBUFFER);
};

struct PointLight {
    PointLight();
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    void transLight(const string &name, Shader* shader);
    void doMovement(unsigned char* KEYBUFFER);
};