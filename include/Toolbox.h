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

typedef shared_ptr<FrameBuffer> FrameBufferPtr;

glm::vec4 getPannel(glm::vec3 p1, glm::vec3 normal, glm::mat4 model);

glm::mat4 getClippedProjectionMatrix(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 clipPlane);
