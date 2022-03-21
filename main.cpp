// TODO 实现调试小窗


#ifndef STB_IMAGE_IMPLEMENTATION // 调用读取图片工具库
    #include "stb_image.h"
#endif

#include "Camera.h"
#include "Global.h"
#include "Render.h"
#include "Play.h"
#include "Scene.h"


/*----------------------------------------------Init---------------------------------------------*/


const int WIDTH = 800;
const int HEIGHT = 800;

float frameTime = 0.0;
float lastFrameTime = 0.0;
Scene* scene;
Play* play;
Render* render;

void display();
void build();
void idle();

void reshape(GLsizei w, GLsizei h) { // 用于窗口变化时将窗口重新设置为原大小
	glViewport(0, 0, w, h);
}


// 键盘操作
void keyboard(unsigned char key, int x, int y, int action) {

	switch (key) {
		case 033: exit(EXIT_SUCCESS); break;
		// default:
			// controlCamera->keyboard(key, x, y, action); break;
	}
}
void keyboardDown(unsigned char key, int x, int y) {
	keyboard(key, x, y, GLFW_PRESS);
}
void keyboardUp(unsigned char key, int x, int y) {
	keyboard(key, x, y, GLFW_RELEASE);
}



int initWindow(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // 窗口支持双重缓冲、深度测试、超采样
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(WIDTH, HEIGHT);
	int mainWindow = glutCreateWindow("Portal");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);	
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(WIDTH / 2, HEIGHT / 2);
	glutIdleFunc(idle);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST); // 开启深度测试
	 (GL_CULL_FACE); // 开启背部剔除
	build();
	return mainWindow;
}	

/*----------------------------------------------Init---------------------------------------------*/


void idle() { // 统计单帧时间
	glutPostRedisplay();
	GLfloat currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
}

void build() { // 搭建场景
	scene = new Scene();
	play = new Play(scene);
	render = new Render(scene);
}


void display() { // 渲染场景
	
}

int main(int argc, char **argv) {
	initWindow(argc, argv);
	// build();
	glutMainLoop(); 
	return 0;
}
