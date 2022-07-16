#pragma once
#include "Model.h"
#include "Global.h"
#include "Camera.h"

class Light {
public:
    virtual void transLight(const string &name, ShaderPtr shader) = 0;
    virtual glm::vec3 doMovement() = 0;
};

class DirLight : public Light {
public:
    DirLight();
    void transLight(const string &name, ShaderPtr shader);
    void transLightCamera(ShaderPtr shader);
    glm::vec3 doMovement();
    void setDirection(glm::vec3 direction);
    glm::vec3 getDirection();
    glm::mat4 getLightViewMatrix();
    CameraPtr lightCamera;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
private:
    glm::vec3 direction;
};

class PointLight : public Light {
public:
    PointLight();
    void transLight(const string &name, ShaderPtr shader);
    glm::vec3 doMovement();
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();
    vector<glm::mat4> getLightViewMatrix();
    void transLightCamera(ShaderPtr shader);
    CameraPtr lightCamera;

    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    /*********** attenuation function **********/
    float constant;
    float linear;
    float quadratic;

private:
    glm::vec3 position;
};

typedef shared_ptr<DirLight> DirLightPtr;
typedef shared_ptr<PointLight> PointLightPtr;