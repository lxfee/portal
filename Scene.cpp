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
        delete obj;
    }
    for(auto camera : cameras) {
        delete camera;
    }
    if(dirLight) delete dirLight;
    if(pointLight) delete pointLight;
}

int Scene::addModel(Model* model) {
    if(!model) return -1;
    objects.push_back(model);   
    return objects.size() - 1;
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


const vector<Camera*>& Scene::getCameras() {
    return cameras;
}

void Scene::setLight(DirLight* _dirLight, PointLight* _pointLight) {
    if(_dirLight) dirLight = _dirLight;
    if(_pointLight) pointLight = _pointLight; 
}

int Scene::addCamera(Camera* camera) {
    if(!camera) return -1;
    cameras.push_back(camera);   
    return cameras.size() - 1;
}

