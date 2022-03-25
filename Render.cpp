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
void FrameBuffer::attachDepth(Texture tex) {
    active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.id, 0);
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

void Render::glClear(GLbitfield mask) { // 代理glClear，避免
    // glEnable(GL_SCISSOR_TEST);
    glScissor(currentWindow.x, currentWindow.y, currentWindow.width, currentWindow.height);
    ::glClear(mask);
    // glDisable(GL_SCISSOR_TEST);
}

Render::Render(Scene* scene, int width, int height) : scene(scene), width(width), height(height), currentWindow(0, 0, width, height) {
    shaders["skyBox"] = new Shader("./shaders/skybox/v.glsl", "./shaders/skybox/f.glsl");
    shaders["shader"] = new Shader("./shaders/normal/v.glsl", "./shaders/normal/f.glsl");
    shaders["shadow"] = new Shader("./shaders/shadow/v.glsl", "./shaders/shadow/f.glsl");
    glClearColor(1, 1, 1, 1); // 设置背景颜色
	glClearDepth(1); // 深度缓冲初始值
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_STENCIL_TEST); // 开启模板测试
	// glEnable(GL_CULL_FACE); // 开启背部剔除，开了天空盒会失效，不需要的时候要关掉

    vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
	};
    Texture shadowMap = Texture::TextureForFramebufferColor("shadowMap", "textureDiffuse", 1024, 1024);
	// Model* floor = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/container.png", "textureDiffuse")})});
	Model* floor = new Model( { Mesh(vertex, {0, 1, 2, 2, 3, 0}, {shadowMap}) } );
    floor->scale = glm::vec3(5);
	// floor->rotation[0] = 90;
    floor->translation = glm::vec3(5, 5, 5);
    fb.attachColor(shadowMap);
    scene->objects["floor"] = floor;
}

void Render::render() {
    auto camera = scene->cameras["camera"];
    auto sCamera = scene->cameras["sCamera"];
    auto dirLight = scene->dirLight;
    Shader* shader;

    
    shadow(Window(0, 0, 1024, 1024));
    


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /******天空盒*****/
    shader = shaders["skyBox"];
    shader->use();
    camera->transCamera(shader);
    shader->setMat4("view", glm::mat4(glm::mat3(camera->getViewMatrix()))); // 天空盒不随视线移动
    scene->objects["skyBox"]->Draw(shader);

    /******场景*****/
    shader = shaders["shader"];
    shader->use();
    shader->setMat4("dirLightViewMatrix",  sCamera->getProjectionMatrix() * sCamera->getViewMatrix());
    // glActiveTexture(GL_TEXTURE0 + 8);
    // shader->setInt("shadowMap", 8);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap.id);
    // glActiveTexture(GL_TEXTURE0);
    dirLight->transLight("dirLight", shader);
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        if(obj.first == "skyBox") continue;
        obj.second->Draw(shader);
    }


    // /******调试窗口*****/
    shadow(Window(width - 300, 0, 300, 300)); 
}


void Render::shadow() {
    
    fb.active();
    glViewport(0, 0, 1024, 1024);
    ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto camera = scene->cameras["sCamera"];
    auto dirLight = scene->dirLight;
    Shader* shader;
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    shader = shaders["shadow"];
    shader->use();
    camera->transCamera(shader);
    for(auto obj : scene->objects) {
        if(obj.first == "skyBox" || obj.first == "floor") continue;
        obj.second->Draw(shader);
    }
    fb.restore();
}
