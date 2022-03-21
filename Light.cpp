#include "Light.h"

// DirLight
DirLight::DirLight(glm::vec3 direction) : direction(direction) {}
void DirLight::setLight(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) {
    ambient = _ambient;
    diffuse = _diffuse;
    specular = _specular;
}


// PointLight
PointLight::PointLight(glm::vec3 position) : position(position), model(nullptr) {}

void PointLight::setLight(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular) {
    ambient = _ambient;
    diffuse = _diffuse;
    specular = _specular;
}

void PointLight::setAttenuation(float _constant, float _linear, float _quadratic) {
    constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}

void PointLight::setModel(Model *_model) {
    model = _model;
}

