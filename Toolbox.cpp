#include "Toolbox.h"

stack<unsigned int> FrameBuffer::fbos;


void FrameBuffer::clear() {
    while(!fbos.empty()) fbos.pop();
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &fbo);
}

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &fbo);
}

void FrameBuffer::attachColor(Texture tex, int channel) {
    active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + channel, GL_TEXTURE_2D, tex.id, 0);
    restore();
}

void FrameBuffer::attachDepth(Texture tex) {
    active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.id, 0);
    restore();
}

void FrameBuffer::attachCubeDepth(Texture tex) {
    active();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex.id, 0);
    restore();
}

void FrameBuffer::attachDepthStencil(Texture tex) {
    active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, tex.id, 0);
    restore();
}

void FrameBuffer::active() {
    fbos.push(fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}


void FrameBuffer::restore() {
    if(fbos.empty()) glBindFramebuffer(GL_FRAMEBUFFER, 0);
    fbos.pop();
    if(fbos.empty()) glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else glBindFramebuffer(GL_FRAMEBUFFER, fbos.top());
}

void FrameBuffer::restore(FrameBuffer* fb) {
    if(!fbos.empty()) fbos.pop();
    fb->active();
}



bool FrameBuffer::CheckComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

glm::vec4 getPannel(glm::vec3 p1, glm::vec3 normal, glm::mat4 model) {
    normal = glm::transpose(glm::inverse(glm::mat3(model))) * normal;
    p1 = glm::vec3(model * glm::vec4(p1, 1.0));
    return glm::vec4(normal, -glm::dot(normal, p1));
}