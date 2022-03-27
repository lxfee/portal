// TODO 编译引用问题

#include "Camera.h"
#include "Global.h"
#include "Render.h"
#include "Scene.h"
#include "Steve.h"

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
Shader* pannelshader, *shader, *depthshader, *skyboxshader;
FrameBuffer* fbo;
Camera* camera;
DirLight* dirLight;
PointLight* pointLight;
Texture TDirDepth;
Texture TPointDepth;
Texture Skybox;
int currentControl;
Steve* him;

const int w = 6000, h = 6000;

void idle() {
	glutPostRedisplay();
	float currentFrameTime = glutGet(GLUT_ELAPSED_TIME);
	frameTime = currentFrameTime - lastFrameTime;
	lastFrameTime= currentFrameTime;
	cout << "\r        \r" << (int)(1000 / frameTime);
	
	// switch(currentControl) {
	// 	case 0: camera->doMovement(KEYBUFFER); break;
	// 	case 1: dirLight->doMovement(KEYBUFFER); break;
	// 	case 2: pointLight->doMovement(KEYBUFFER); break;
	// }
	camera->doMovement(KEYBUFFER);
	him->doMovement(KEYBUFFER);
}

void build() {
	scene = new Scene();
	glutWarpPointer(MIDWIDTH, MIDHEIGHT);
	
	
	glClearColor(1, 1, 1, 1); 	// 设置背景颜色
	glClearDepth(1); 			// 深度缓冲初始值
    glEnable(GL_DEPTH_TEST); 	// 开启深度测试
    glEnable(GL_STENCIL_TEST); 	// 开启模板测试
	
	vector<string> skyBoxPath = {
        "./models/skybox/right.jpg",
        "./models/skybox/left.jpg",
	    "./models/skybox/top.jpg",
	    "./models/skybox/bottom.jpg",
        "./models/skybox/front.jpg",
        "./models/skybox/back.jpg"
    };

	vector<Vertex> vertex = {
		{glm::vec3( 0,  0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3( -1, 0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-1, -1, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0,  -1, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
	};
	TDirDepth = Texture::TextureForFramebufferDepth("dirDepMap", w, h);
	TPointDepth = Texture::TextureForFramebufferDepth("pointDepMap", w, h);
	Skybox = Texture::CubeTextureFromFile(skyBoxPath, "cubeTexture");
	// pannel = new Model({ Mesh(vertex, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/floor.png")}) });
	pannel = new Model({ Mesh(vertex, {0, 1, 2, 2, 3, 0}, { TPointDepth }) });
	
	pannelshader = new Shader("./shaders/simple/v.glsl", "./shaders/simple/f.glsl");
	shader = new Shader("./shaders/normal/v.glsl", "./shaders/normal/f.glsl");
	depthshader = new Shader("./shaders/shadow/v.glsl", "./shaders/shadow/f.glsl");
	skyboxshader = new Shader("./shaders/skybox/v.glsl", "./shaders/skybox/f.glsl");
	dirLight = new DirLight();
	pointLight = new PointLight();
	him = new Steve();
	camera = new Camera();
	
	fbo = new FrameBuffer();
}


void display() { 



	// fbo->attachDepth(TDirDepth);
	// fbo->active();
	// glViewport(0, 0, w, h);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// depthshader->use();
	// dirLight->lightCamera->transCamera(depthshader);
	// for(auto& obj : scene->objects) {
	// 	if(obj.first == "skybox" || obj.first == "floor") continue;
	// 	obj.second->Draw(depthshader);
	// }
	// fbo->restore();

	// fbo->attachDepth(TPointDepth);
	// fbo->active();
	// glViewport(0, 0, w, h);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// depthshader->use();
	// pointLight->lightCamera->transCamera(depthshader);
	// for(auto& obj : scene->objects) {
	// 	if(obj.first == "skybox") continue;
	// 	obj.second->Draw(depthshader);
	// }
	// fbo->restore();

	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	skyboxshader->use();
	camera->transCamera(skyboxshader);
	Skybox.transTexture(shader);
	scene->objects["skybox"]->Draw(skyboxshader);
	
	
	shader->use();
	shader->setMat4("dirLightViewMatrix",  dirLight->getLightViewMatrix());
	shader->setMat4("pointLightViewMatrix",  pointLight->getLightViewMatrix());
	camera->transCamera(shader);
	dirLight->transLight("", shader);
	pointLight->transLight("", shader);
	TDirDepth.transTexture(shader, 8);
	TPointDepth.transTexture(shader, 9);
	for(auto& obj : scene->objects) {
		if(obj.first == "skybox") continue;
		obj.second->Draw(shader);
	}
	him->Draw(shader);

	// glClear(GL_DEPTH_BUFFER_BIT);
	// pannelshader->use();
	// pannel->Draw(pannelshader);

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
	camera->mouseWheel(button, dir, x, y);
}


void mouseMotion(int x, int y) {
	static int lastX = MIDWIDTH, lastY = MIDHEIGHT;
	float deltaX = x - lastX;
	float deltaY = lastY - y; // 注意这里是相反的，因为y坐标的范围是从下往上的
	camera->mouseMotion(deltaX, deltaY);
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
