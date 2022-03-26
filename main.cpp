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
static unsigned char KEYBUFFER[1024];



Scene* scene;
Model* pannel;
Shader* pannelshader;
Shader* shader;
Shader* depthshader;
FrameBuffer* fbo;
Camera* curCamera;
Camera* camera;
Camera* dirDepCamera;
Camera* pointDepCamera;
DirLight* dirLight;
PointLight* pointLight;
Texture TDirDepth;
Texture TPointDepth;
int currentControl;
const int w = 5000, h = 5000;

void idle() {
	glutPostRedisplay();
	GLfloat currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime= currentFrameTime;
	cout << "\r        \r" << (int)(1000 / frameTime);
	
	switch(currentControl) {
		case 0: curCamera = camera; break;
		case 1: curCamera = pointDepCamera; break;
		case 2: dirLight->doMovement(KEYBUFFER); break;
	}

	curCamera->doMovement(KEYBUFFER);
	pointLight->position = pointDepCamera->eye;
	dirDepCamera->dir = dirLight->direction;
}

void build() {
	scene = new Scene();
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
	
	
	glClearColor(1, 1, 1, 1); 	// 设置背景颜色
	glClearDepth(1); 			// 深度缓冲初始值
    glEnable(GL_DEPTH_TEST); 	// 开启深度测试
    glEnable(GL_STENCIL_TEST); 	// 开启模板测试

	vector<Vertex> vertex = {
		{glm::vec3( 0,  0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3( -1, 0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-1, -1, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0,  -1, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
	};
	TDirDepth = Texture::TextureForFramebufferDepth("depth", "textureDiffuse", w, h);
	TPointDepth = Texture::TextureForFramebufferDepth("depth", "textureDiffuse", w, h);
	// pannel = new Model({ Mesh(vertex, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/floor.png", "textureDiffuse")}) });
	pannel = new Model({ Mesh(vertex, {0, 1, 2, 2, 3, 0}, { TPointDepth }) });
	
	pannelshader = new Shader("./shaders/simple/v.glsl", "./shaders/simple/f.glsl");
	shader = new Shader("./shaders/normal/v.glsl", "./shaders/normal/f.glsl");
	depthshader = new Shader("./shaders/shadow/v.glsl", "./shaders/shadow/f.glsl");
	camera = new Camera();
	pointDepCamera = new Camera();
	pointDepCamera->eye = glm::vec3(0, 20, 0);
	pointDepCamera->dir = glm::vec3(0, -1, 0);
	pointDepCamera->aspect = 1.0 * w / h;
	pointDepCamera->fov = 90;
	curCamera = pointDepCamera;
	dirDepCamera = new Camera();
	dirDepCamera->projMode = ORTHO;
	dirDepCamera->eye = glm::vec3(0, 20, 0);
	dirDepCamera->dir = glm::vec3(0, -1, 0);
	dirLight = new DirLight();
	pointLight = new PointLight();
	fbo = new FrameBuffer();
}


void display() { 
	fbo->attachDepth(TDirDepth);
	fbo->active();
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	depthshader->use();
	dirDepCamera->transCamera(depthshader);
	dirDepCamera->scale = 50;
	for(auto& obj : scene->objects) {
		if(obj.first == "skyBox" || obj.first == "floor") continue;
		obj.second->Draw(depthshader);
	}
	fbo->restore();

	fbo->attachDepth(TPointDepth);
	fbo->active();
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	depthshader->use();
	pointDepCamera->transCamera(depthshader);
	for(auto& obj : scene->objects) {
		if(obj.first == "skyBox") continue;
		obj.second->Draw(depthshader);
	}
	fbo->restore();

	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->use();
	shader->setMat4("dirLightViewMatrix",  dirDepCamera->getProjectionMatrix() * dirDepCamera->getViewMatrix());
	shader->setMat4("pointLightViewMatrix",  pointDepCamera->getProjectionMatrix() * pointDepCamera->getViewMatrix());
	camera->transCamera(shader);
	dirLight->transLight("dirLight", shader);
	pointLight->transLight("pointLight", shader);
	
	glActiveTexture(GL_TEXTURE0 + 8);
    shader->setInt("dirDepMap", 8);
    glBindTexture(GL_TEXTURE_2D, TDirDepth.id);
    glActiveTexture(GL_TEXTURE0);
	
	glActiveTexture(GL_TEXTURE0 + 9);
    shader->setInt("pointDepMap", 9);
    glBindTexture(GL_TEXTURE_2D, TPointDepth.id);
    glActiveTexture(GL_TEXTURE0);

	for(auto& obj : scene->objects) {
		if(obj.first == "skyBox") continue;
		obj.second->Draw(shader);
	}

	glClear(GL_DEPTH_BUFFER_BIT);
	pannelshader->use();
	pannel->Draw(pannelshader);

	glutSwapBuffers(); // 双缓冲，减少闪烁
}






/*----------------------------------------------回调函数---------------------------------------------*/
void keyboardDown(unsigned char key, int x, int y) {
	switch(key) {
		case 033: exit(EXIT_SUCCESS); break;
		case '=': currentControl = (currentControl + 1) % 3; break;
	}
	KEYBUFFER[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    KEYBUFFER[key] = false;
}

void mouseWheel(int button, int dir, int x, int y) {
	curCamera->mouseWheel(button, dir, x, y);
}


void mouseMotion(int x, int y) {
	static int lastX = MIDWIDTH, lastY = MIDHEIGHT;
	float deltaX = x - lastX;
	float deltaY = lastY - y; // 注意这里是相反的，因为y坐标的范围是从下往上的
	curCamera->mouseMotion(deltaX, deltaY);
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
