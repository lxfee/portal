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

void Render::glClear(GLbitfield mask) { // 代理glClear，避免
    glEnable(GL_SCISSOR_TEST);
    glScissor(currentWindow.x, currentWindow.y, currentWindow.width, currentWindow.height);
    ::glClear(mask);
    glDisable(GL_SCISSOR_TEST);
}

Render::Render(Scene* scene, int width, int height) : scene(scene), width(width), height(height), currentWindow(0, 0, width, height) {
    Shader* skyBoxShader = new Shader("./shaders/vSkyBox.glsl", "./shaders/fSkyBox.glsl");
    Shader* gshader = new Shader("./shaders/vshader.glsl", "./shaders/fshader.glsl", "./shaders/gshader.glsl");
    shaders["skyBox"] = skyBoxShader;
    shaders["shader"] = gshader;
    
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置背景颜色
    glClearColor(0.5, 0.5, 0.5, 1); // 设置背景颜色
	glClearDepth(1); // 深度缓冲初始值
    glEnable(GL_DEPTH_TEST); // 开启深度测试
    glEnable(GL_STENCIL_TEST); // 开启模板测试
	glEnable(GL_CULL_FACE); // 开启背部剔除，开了天空盒会失效，不需要的时候要关掉

    amount = 1000000;
    modelMatrices = new glm::mat4[amount];
    srand(glutGet(GLUT_ELAPSED_TIME)); // 初始化随机种子    
    float radius = 20.0;
    float offset = 2.5f;
    for(unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model(1);
        // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. 缩放：在 0.05 和 0.25f 之间缩放
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. 添加到矩阵的数组中
        modelMatrices[i] = model;
    }  
    scene->objects["cube"]->setInstanceMatrix(3, amount, modelMatrices);
}

void Render::render() {
    auto camera = scene->cameras["camera"];
    auto dirLight = scene->dirLight;
    Shader* shader;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // shader = shaders["skyBox"];
    // shader->use();
    // camera->transCamera(shader);
    // shader->setMat4("view", glm::mat4(glm::mat3(camera->getViewMatrix()))); // 天空盒不随视线移动
    // shader->setMat4("model", scene->objects["skyBox"]->getModelMatrix());
    // scene->objects["skyBox"]->Draw(shader);

    shader = shaders["shader"];
    shader->use();
    dirLight->transLight("dirLight", shader);
    camera->transCamera(shader);
    auto cube = scene->objects["cube"];
    // for(int i = 0 ; i < amount; i++) {
        // shader->setMat4("model", modelMatrices[i]);
        cube->Draw(shader, amount);
    // }

    
}


void Render::debug() {

}
