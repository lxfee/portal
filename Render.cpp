#include "Render.h"


FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &fbo);
}

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &fbo);
    // 深度和模板渲染对象rbo，只写不读，只用于测试，优化速度
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    active();
    // 绑定渲染对象到帧缓冲上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    restore();
}
void FrameBuffer::attachColor(Texture tex) {
    active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.id, 0);
    restore();
}
void FrameBuffer::attachDepthStencil(Texture tex) {
    active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.id, 0);
    restore();
}
void FrameBuffer::active() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
void FrameBuffer::restore() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::CheckComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

Render::~Render() {
    for(auto shader : shaders) {
        delete shader.second;
    }
    delete scene;
}

void Render::glClear(GLbitfield mask) {
    glEnable(GL_SCISSOR_TEST);
    glScissor(currentWindow.x, currentWindow.y, currentWindow.width, currentWindow.height);
    ::glClear(mask);
    glDisable(GL_SCISSOR_TEST);
}

Render::Render(Scene* scene, int width, int height) : scene(scene), width(width), height(height), currentWindow(0, 0, width, height) {
    Shader* shader = new Shader("./shaders/vshader.glsl", "./shaders/fshader.glsl");
    Shader* cshader = new Shader("./shaders/vshader.glsl", "./shaders/color.glsl");
    shaders["shader"] = shader;
    shaders["color"] = cshader;
    
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置背景颜色
    glClearColor(0.5, 0.5, 0.5, 1); // 设置背景颜色
	glClearDepth(1); // 深度缓冲初始值
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_STENCIL_TEST); // 开启模板测试
	glEnable(GL_CULL_FACE); // 开启背部剔除
    
}

void Render::render() {
    auto camera = scene->cameras["camera"];
    Shader* shader = shaders["shader"];
    auto dirLight = scene->dirLight;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    dirLight->transLight("dirLight", shader);
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        obj.second->Draw(shader);
    }
}


void Render::debug() {
    auto camera = scene->cameras["camera"];
    Shader* shader = shaders["color"];
    auto dirLight = scene->dirLight;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    dirLight->transLight("dirLight", shader);
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        obj.second->Draw(shader);
    }
}
