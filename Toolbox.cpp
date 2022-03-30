#include "Toolbox.h"


FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &fbo);
}

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &fbo);
}

// attach 一定要在active之前
void FrameBuffer::attachColor(Texture tex, int channel) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + channel, GL_TEXTURE_2D, tex.id, 0);
}

// attach 一定要在active之后
void FrameBuffer::attachDepth(Texture tex) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.id, 0);
}

// attach 一定要在active之后
void FrameBuffer::attachDepthStencil(Texture tex) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL, GL_TEXTURE_2D, tex.id, 0);
}


void FrameBuffer::active() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}


void FrameBuffer::restore(const FrameBuffer* fb) {
    if(fb) glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    else glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::CheckComplete() {
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}