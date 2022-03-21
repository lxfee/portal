#pragma once
#include "Model.h"
#include "Global.h"


class DirLight {
    public:
        DirLight(glm::vec3 direction);
        void setLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    private:
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
};

class PointLight {
    public:
        PointLight(glm::vec3 position);
        void setAttenuation(float _constant, float _linear, float _quadratic);
        void setModel(Model *model);
        void setLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    private:
        glm::vec3 position;  
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float constant;
        float linear;
        float quadratic;
        Model *model;
};