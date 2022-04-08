#pragma once
#include "Global.h"
#include "Texture.h"

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        void active();
        void attachColor(Texture tex, int channel = 0);
        void attachDepth(Texture tex);
        void attachCubeDepth(Texture tex);
        void attachDepthStencil(Texture tex);
        static void restore(FrameBuffer* fb);
        static void restore();
        bool CheckComplete();
        static stack<unsigned int> fbos;
        static void clear();
    private:
        unsigned int fbo;
};

glm::vec4 getPannel(glm::vec3 p1, glm::vec3 normal, glm::mat4 model);