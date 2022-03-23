#include "Render.h"

Render::~Render() {
    for(auto shader : shaders) {
        delete shader;
    }
}

Render::Render(Scene* scene) : scene(scene) {
    Shader* shader = new Shader("./shaders/vshader.glsl", "./shaders/fshader.glsl");
    Shader* lshader = new Shader("./shaders/vshader.glsl", "./shaders/flight.glsl");
    shaders.push_back(shader);
    shaders.push_back(lshader);
}

void Render::render() {
    auto camera = scene->cameras[0];
    auto shader = shaders[0];
    auto lshader = shaders[1];
    auto pointLight = scene->pointLights[0];
    auto lightCube = scene->objects[0];
    lightCube->translation = pointLight->position;
    
    lshader->use();
    camera->transCamera(lshader);
    lightCube->Draw(lshader);

    shader->use();
    pointLight->transLight("pointLight", shader);
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        if(obj == lightCube) continue;
        obj->Draw(shader);
    }
}