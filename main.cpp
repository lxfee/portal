// TODO 实现调试小窗
// TODO 编译引用问题

#include "Camera.h"
#include "Global.h"
#include "Render.h"
#include "Play.h"
#include "Scene.h"

/*----------------------------------------------Init---------------------------------------------*/


const int WIDTH = 800;
const int HEIGHT = 800;
const int MIDWIDTH  = WIDTH / 2;
const int MIDHEIGHT = HEIGHT / 2;

float frameTime = 0.0;
float lastFrameTime = 0.0;

Scene* scene;
Play* play;
Render* render;

void display();
void build();
void idle();

void reshape(GLsizei w, GLsizei h) { glViewport(0, 0, w, h); }
void keyboardDown(unsigned char key, int x, int y) {if(key == 033) exit(EXIT_SUCCESS); play->keyboard(key, x, y, GLFW_PRESS);}
void keyboardUp(unsigned char key, int x, int y) {play->keyboard(key, x, y, GLFW_RELEASE);}
void mouseWheel(int button, int dir, int x, int y) {play->mouseWheel(button, dir, x, y);}
void mouseMotion(int x, int y) {
	static int lastX = MIDWIDTH, lastY = MIDHEIGHT;
	float deltaX = x - lastX;
	float deltaY = lastY - y; // 注意这里是相反的，因为y坐标的范围是从下往上的
	play->mouseMotion(deltaX, deltaY);
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
	
}

void init(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // 窗口支持双重缓冲、深度测试、超采样
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(WIDTH, HEIGHT);
	int mainWindow = glutCreateWindow("Portal");
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	

	glewExperimental = GL_TRUE;
	glewInit(); // glew 要在 glut 初始化后初始化
	// glEnable(GL_DEPTH_TEST); // 开启深度测试
	// glEnable(GL_CULL_FACE); // 开启背部剔除

	build(); // build中用到glew的函数，需要在glew初始化后初始化
	
	
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseWheelFunc(mouseWheel);
	glutPassiveMotionFunc(mouseMotion);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);	
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
	// glutSetCursor(GLUT_CURSOR_NONE);
}	

/*----------------------------------------------Init---------------------------------------------*/


void idle() { // 统计单帧时间
	glutPostRedisplay();
	GLfloat currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	play->idle(frameTime);
}

Mesh* mesh;
Shader* shader;

vector<Vertex> vex = {
	Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec2(0)},
	Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0), glm::vec2(0)},
	Vertex{glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0), glm::vec2(0)},
};

void build() { // 搭建场景

	Model* obj = new Model("../models/namo/nanosuit.obj");
	Camera* camera = new Camera();
	scene = new Scene();
	scene->addModel(obj);
	scene->addCamera(camera);
	render = new Render(scene);
	play = new Play(scene);

	mesh = new Mesh(vex, {0, 1, 2}, vector<Texture>());
    shader = new Shader("../shaders/vshader.glsl", "../shaders/fshader.glsl");
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
	
}


void display() { // 渲染场景
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置背景颜色
	glClear(GL_COLOR_BUFFER_BIT);
	
	auto camera = scene->getCameras().front();
	shader->use();
	shader->setMat4("vp", camera->getProjectionMatrix() * camera->getViewMatrix());
	mesh->Draw(shader);

	render->render();
	
	glutSwapBuffers(); // 双缓冲，减少闪烁
}

int main(int argc, char **argv) {
	init(argc, argv);
	glutMainLoop(); 
	return 0;
}
