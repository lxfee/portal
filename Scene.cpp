#include "Scene.h"

Scene::Scene() {
    dirLight = nullptr;
    pointLight = nullptr;
}

Scene::Scene(DirLight* _dirLight, PointLight* _pointLight) {
    dirLight = _dirLight;
    pointLight = _pointLight;   
}

Scene::~Scene() {
    for(auto obj : objects) {
        if(obj) delete obj;
    }
    if(dirLight) delete dirLight;
    if(pointLight) delete pointLight;
}

void Scene::addModel(Model* model) {
    objects.push_back(model);   
}

DirLight * Scene::getDirLight() {
    return dirLight;
}
PointLight* Scene::getPointLight() {
    return pointLight;
}

const vector<Model*>& Scene::getObjects() {
    return objects;
}

