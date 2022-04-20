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


glm::mat4 getClippedProjectionMatrix(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 clipPlane) { //四维向量表示门平面的解析式，向量+点
	clipPlane = glm::transpose(glm::inverse(viewMat)) * clipPlane;
	glm::vec4 q;
	q.x = (glm::sign(clipPlane.x) + projMat[2][0]) / projMat[0][0];
	q.y = (glm::sign(clipPlane.y) + projMat[2][1]) / projMat[1][1];
	q.z = -1.0F;
	q.w = (1.0F + projMat[2][2]) / projMat[3][2];
	glm::vec4 c = clipPlane * (2.0f / glm::dot(clipPlane, q));
	projMat[0][2] = c.x;
	projMat[1][2] = c.y;
	projMat[2][2] = c.z + 1.0F;
	projMat[3][2] = c.w;
	return projMat;
}


glm::vec4 getPannel(glm::vec3 p1, glm::vec3 normal, glm::mat4 model) {
    normal = glm::transpose(glm::inverse(glm::mat3(model))) * normal;
    p1 = glm::vec3(model * glm::vec4(p1, 1.0));
    return glm::vec4(normal, -glm::dot(normal, p1));
}