#include "Render.h"

Render::~Render() {
    for(auto shader : shaders) {
        delete shader.second;
    }
}

Render::Render(Scene* scene) : scene(scene) {
    Shader* shader = new Shader("./shaders/vshader.glsl", "./shaders/fshader.glsl");
    Shader* cshader = new Shader("./shaders/vshader.glsl", "./shaders/color.glsl");
    shaders["shader"] = shader;
    shaders["color"] = cshader;
    
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置背景颜色
    glClearColor(0.5, 0.5, 0.5, 1); // 设置背景颜色
	glClearDepth(1); // 深度缓冲初始值
    glClearStencil(0);
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_STENCIL_TEST); // 开启模板测试
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // 设置模板测试通过模式
	glEnable(GL_CULL_FACE); // 开启背部剔除

    vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};
    Texture tex;
	auto id = Model::TextureFromFile("container.png", "./models/");
	tex.id = id; tex.type = "textureDiffuse";
	floor = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {tex})});
    floor->scale = glm::vec3(1000, 1000, 1000);
	floor->rotation[0] = 90;
    
}

void Render::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // 注意清理深度缓冲
    
    auto camera = scene->cameras[0];
    Shader* shader;
    auto pointLight = scene->pointLights[0];
    auto dirLight = scene->dirLight;
    
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
    glStencilMask(0xFF); // 启用模板缓冲写入
    shader = shaders["shader"];
    /************绘制场景**********/
    shader->use();
    dirLight->transLight("dirLight", shader);
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        obj->scale = glm::vec3(1);
        obj->Draw(shader);
    }
    
    /************绘制场景**********/

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // 禁止模板缓冲的写入
    floor->Draw(shader);
    glDisable(GL_DEPTH_TEST);
    shader = shaders["color"];
    /************绘制场景**********/
    shader->use();
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        obj->scale = glm::vec3(1.1);
        obj->Draw(shader);
    }
    /************绘制场景**********/
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF); 

}