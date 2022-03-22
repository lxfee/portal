#include "Render.h"

Render::~Render() {
    for(auto shader : shaders) {
        delete shader;
    }
}

Render::Render(Scene* scene) : scene(scene) {
    Shader* shader = new Shader("./shaders/vshader.glsl", "./shaders/fshader.glsl");
    shaders.push_back(shader);
}

void Render::render() {
    auto camera = scene->getCameras().front();
    auto obj = scene->getObjects().front();
    auto shader = shaders.front();
    shader->setMat4("vp", camera->getProjectionMatrix() * camera->getViewMatrix());
    
    shader->use();
    obj->Draw(shader);
}