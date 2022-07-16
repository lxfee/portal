#include "Camera.h"
#include "Global.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "Render.h"

int WIDTH = 1280;
int HEIGHT = 720;
int MIDWIDTH  = WIDTH / 2;
int MIDHEIGHT = HEIGHT / 2;
float frameTime = 0;
float lastFrameTime = 0;
unsigned char KEYBUFFER[1024];


ScenePtr scene;
Render* render;

void printfps() {
	float currentFrameTime = (float)glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime= currentFrameTime;
	cout << "\r        \r" << (int)(1000 / frameTime);
}



void build() {
	scene = make_shared<Scene>();
	render = new Render(scene);
}


void display() { 
	render->masterRender();
	glutSwapBuffers();
}

void idle() {
	glutPostRedisplay();
	printfps();

   	scene->masterCamera->doMovement();
}


void keyboardDown(unsigned char key, int x, int y) {
	switch(key) {
		case 033: exit(EXIT_SUCCESS); break;
	}
	KEYBUFFER[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    KEYBUFFER[key] = false;
}

void mouseWheel(int button, int dir, int x, int y) {
	scene->masterCamera->mouseWheel(button, dir, x, y);
}

void mouseMotion(int x, int y) {
	static int lastX = MIDWIDTH, lastY = MIDHEIGHT;
	static float mouseDeltaX = 0, mouseDeltaY = 0;
	mouseDeltaX = (float)x - lastX;
	mouseDeltaY = lastY - (float)y;
	scene->masterCamera->mouseMotion(mouseDeltaX, mouseDeltaY);
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // 窗口支持双重缓冲、深度测试、超采样
	int mainWindow = glutCreateWindow("Portal");
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseWheelFunc(mouseWheel);
	glutPassiveMotionFunc(mouseMotion);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);	
	glewExperimental = GL_TRUE;
	glewInit(); // glew must be init beyond glut
	build(); // build calls glew funcation, must be called after glew has been initialized
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
	glutMainLoop(); 
	return 0;
}
