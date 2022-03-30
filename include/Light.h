#pragma once
#include "Model.h"
#include "Global.h"
#include "Camera.h"

class Light {
public:
    ~Light();
    glm::mat4 getLightViewMatrix();                                     // 获得光相机视矩阵
    virtual void transLight(const string &name, Shader* shader) = 0;    // 向着色器传入光照信息
    virtual glm::vec3 doMovement() = 0;              // 光移动函数
    Camera* lightCamera;
};

class DirLight : public Light {
public:
    DirLight();
    void transLight(const string &name, Shader* shader);
    glm::vec3 doMovement();
    void setDirection(glm::vec3 direction);
    glm::vec3 getDirection();

    glm::vec3 ambient;      // 环境光系数
    glm::vec3 diffuse;      // 漫反射系数
    glm::vec3 specular;     // 高光系数
private:
    glm::vec3 direction;    // 方向
};

class PointLight : public Light {
public:
    PointLight();
    void transLight(const string &name, Shader* shader);
    glm::vec3 doMovement();
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();

    glm::vec3 ambient;      // 环境光系数 
    glm::vec3 diffuse;      // 漫反射系数
    glm::vec3 specular;     // 高光系数
    /*********** 距离衰减参数 **********/
    float constant;         // 常数项
    float linear;           // 一次项
    float quadratic;        // 二次项
private:
    glm::vec3 position;     // 位置
};