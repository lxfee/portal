#pragma once

#include "Global.h"
#include "Scene.h"

struct Window {
    Window(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
    int x, y, width, height;
};

class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();
        void attachColor(Texture tex);
        void attachDepth(Texture tex);
        void active();
        static void restore();
        bool CheckComplete();
    private:
        unsigned int fbo;
};

class Render {
    public:
        ~Render();
        Render(Scene* scene, int width, int height);
        void render(Window window) {
            currentWindow = window;
            glViewport(window.x, window.y, window.width, window.height);
            render();
            currentWindow = Window(0, 0, width, height);
            glViewport(0, 0, width, height);
        }
        void render();   
        
        void shadow(Window window) {
            currentWindow = window;
            glViewport(window.x, window.y, window.width, window.height);
            shadow();
            currentWindow = Window(0, 0, width, height);
            glViewport(0, 0, width, height);
        }
        void shadow();

    private:
        Scene* scene;
        map<string, Shader*> shaders;
        // FrameBuffer fb; 
        Texture shadowMap; // 临时
        void glClear(GLbitfield mask); // 使用裁剪和视口缩放，实现子窗口
        int width, height;
        Window currentWindow;
};
