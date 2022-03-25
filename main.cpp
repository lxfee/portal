// TODO 编译引用问题

#include "Camera.h"
#include "Global.h"
#include "Render.h"
#include "Play.h"
#include "Scene.h"

/*----------------------------------------------定义---------------------------------------------*/
const int WIDTH = 1280;
const int HEIGHT = 720;
const int MIDWIDTH  = WIDTH / 2;
const int MIDHEIGHT = HEIGHT / 2;

float frameTime = 0;
float lastFrameTime = 0;
bool DEBUG;

Scene* scene;
Play* play;
Render* render;


/*----------------------------------------------统计单帧时间---------------------------------------------*/
void idle() {
	glutPostRedisplay();
	GLfloat currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime= currentFrameTime;
	cout << "\r        \r" << (int)(1000 / frameTime);
	play->idle();
}






/*----------------------------------------------搭建场景---------------------------------------------*/
void build() {
	scene = new Scene();
	render = new Render(scene, WIDTH, HEIGHT);
	play = new Play(scene);
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
}







/*----------------------------------------------display---------------------------------------------*/

void display() { 
	render->render();
	if(DEBUG) render->debug(Window(WIDTH-200, 0, 200, 200));
	glutSwapBuffers(); // 双缓冲，减少闪烁
}






/*----------------------------------------------回调函数---------------------------------------------*/
void keyboardDown(unsigned char key, int x, int y) {
	switch(key) {
		case 033: exit(EXIT_SUCCESS); 
		case '=': DEBUG ^= 1; break;
		default: play->keyboard(key, x, y, GLFW_PRESS);
	}		
}

void keyboardUp(unsigned char key, int x, int y) {play->keyboard(key, x, y, GLFW_RELEASE);}
void mouseWheel(int button, int dir, int x, int y) {play->mouseWheel(button, dir, x, y);}
void mouseMotion(int x, int y) {
	static int lastX = MIDWIDTH, lastY = MIDHEIGHT;
	float deltaX = x - lastX;
	float deltaY = lastY - y; // 注意这里是相反的，因为y坐标的范围是从下往上的
	play->mouseMotion(deltaX, deltaY);
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
	
	glutMainLoop(); 
	return 0;
}
