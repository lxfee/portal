// TODO 编译引用问题
// TODO 分离操作，操作存放缓冲区，全局变量

#include "Camera.h"
#include "Global.h"
#include "Toolbox.h"
#include "Scene.h"
#include "Steve.h"
#include "Texture.h"
#include "Toolbox.h"
#include "Render.h"
#include "Game.h"

/*-------------------------------------------全局变量&定义---------------------------------------------*/
int WIDTH = 1280;
int HEIGHT = 720;
int MIDWIDTH  = WIDTH / 2;
int MIDHEIGHT = HEIGHT / 2;
float frameTime = 0;
float lastFrameTime = 0;
unsigned char KEYBUFFER[1024];			// 键盘输入缓冲


ScenePtr scene;
Render* render;
Game* game;



/*----------------------------------------------回调函数---------------------------------------------*/

void build() {
	scene = make_shared<Scene>();
	render = new Render(scene);
	game = new Game(scene);
}


void display() { 
	render->masterRender();
	glutSwapBuffers(); // 双缓冲，减少闪烁
}

void idle() {
	glutPostRedisplay();
	float currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime= currentFrameTime;
	cout << "\r        \r" << (int)(1000 / frameTime);
	game->idle();
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
	game->mouseWheel(button, dir, x, y);
}

void mouseMotion(int x, int y) {
	static int lastX = MIDWIDTH, lastY = MIDHEIGHT;
	static float mouseDeltaX = 0, mouseDeltaY = 0;
	mouseDeltaX = x - lastX;
	mouseDeltaY = lastY - y; // 注意这里是相反的，因为y坐标的范围是从下往上的
	game->mouseMotion(mouseDeltaX, mouseDeltaY);
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
	glewInit(); // glew 要在 glut 初始化后初始化
	build(); // build中用到glew的函数，需要在glew初始化后初始化
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
	glutMainLoop(); 
	return 0;
}
