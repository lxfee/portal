#pragma once
#include "Global.h"
#include "Texture.h"

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        void active();
        static void attachColor(Texture tex, int channel = 0);
        static void attachDepth(Texture tex);
        static void attachDepthStencil(Texture tex);
        static void restore(const FrameBuffer* fb = nullptr);
        bool CheckComplete();
    private:
        unsigned int fbo;
};