#pragma once
#include "Model.h"
#include "Global.h"
#include "Camera.h"

class DirLight {
public:
    DirLight();
    void transLight(const string &name, ShaderPtr shader);
    void transLightCamera(ShaderPtr shader);
    void setDirection(glm::vec3 direction);

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
private:
    glm::vec3 direction;
    glm::vec3 position;
    float scale;
    float viewfar, viewnear;
    
    void updateLightViewMatrix();
    glm::mat4 lightViewMatrix;
};

class PointLight {
public:
    PointLight();
    void transLight(const string &name, ShaderPtr shader);
    void setPosition(glm::vec3 position);
    void transLightCamera(ShaderPtr shader);

    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

private:
    glm::vec3 position;
    float viewfar, viewnear;

    void updateLightViewMatrix();
    vector<glm::mat4> lightViewMatrixs;
};

typedef shared_ptr<DirLight> DirLightPtr;
typedef shared_ptr<PointLight> PointLightPtr;