#include "Render.h"



void Render::sceneRender(const FrameBuffer* fbo) {
    auto& TDirDepth = scene->TDirDepth;
    extern const int WIDTH;
    extern const int HEIGHT;
    depthFbo->active();
	depthFbo->attachDepth(TDirDepth);
	glViewport(0, 0, TDirDepth.width, TDirDepth.height);
	glClear(GL_DEPTH_BUFFER_BIT);
	depthShader->use();
	scene->dirLight->lightCamera->transCamera(depthShader);
	for(auto& obj : scene->objects) {
		obj->Draw(depthShader);
	}
	scene->steve->Draw(depthShader);
	depthFbo->restore(fbo);

	
	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	skyboxShader->use();
	scene->Tskybox.transTexture(skyboxShader);
	scene->masterCamera->transCamera(skyboxShader);
	
	scene->skybox->Draw(skyboxShader);

	renderLightCube();
	
	masterShader->use();
	scene->masterCamera->transCamera(masterShader);
	masterShader->setInt("pointLightNumber", scene->pointLights.size());
	for(int i = 0; i < scene->pointLights.size(); i++) {
		scene->pointLights[i]->transLight(("[" + to_string(i) + "]").c_str(), masterShader);
	}
	scene->dirLight->transLight("", masterShader);
	masterShader->setMat4("dirLightViewMatrix", scene->dirLight->getLightViewMatrix());
	TDirDepth.transTexture(masterShader, 8);
	for(auto& obj : scene->objects) {
		obj->Draw(masterShader);
	}
	scene->steve->Draw(masterShader);
}

void Render::renderGlass() {
	glassShader->use();
	scene->TC.transTexture(glassShader, 10);
	scene->masterCamera->transCamera(glassShader);
	for(auto& glass : scene->glasses) {
        glass->Draw(glassShader);
    }
}

void Render::renderLightCube() {
    auto cube = scene->cube;
    basicShader->use();
	scene->masterCamera->transCamera(basicShader);
	cube->scale = glm::vec3(0.2);
	for(auto& pointLight : scene->pointLights) {
		basicShader->setVec3("color", pointLight->ambient);
		cube->translation = pointLight->getPosition();
		cube->Draw(basicShader);
	}
}

void Render::debugRender() {
    glClear(GL_DEPTH_BUFFER_BIT);
	debugShader->use();
	scene->TC.transTexture(debugShader, 10);
	scene->debugPannel->Draw(debugShader);
}

void Render::renderLine(glm::vec3 p1, glm::vec3 p2, int lineWide) {
    basicShader->use();
    basicShader->setVec3("color", glm::vec3(1, 0, 0));

    glLineWidth(lineWide);				// 设置线宽
	static bool isFirst = true;
	static unsigned int VAO, VBO, EBO;
	static unsigned int ind[2] = {0, 1};
	glm::vec3 vec[2] = {p1, p2};
	if(isFirst) {
		glGenVertexArrays(1, &VAO);
    	glGenBuffers(1, &VBO);
    	glGenBuffers(1, &EBO);
		isFirst = false;
	}
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec3), &vec[0], GL_STATIC_DRAW);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Render::Render(Scene* scene) : scene(scene) {
    glClearColor(0.5, 0.5, 0.5, 1); 	// 设置背景颜色
	glClearDepth(1); 					// 深度缓冲初始值
	glEnable(GL_DEPTH_TEST);			// 开启深度测试
    debugShader = new Shader("./shaders/Debug/v.glsl", "./shaders/Debug/f.glsl");
	masterShader = new Shader("./shaders/Master/v.glsl", "./shaders/Master/f.glsl");
	depthShader = new Shader("./shaders/Shadow/v.glsl", "./shaders/Shadow/f.glsl");
	skyboxShader = new Shader("./shaders/Skybox/v.glsl", "./shaders/Skybox/f.glsl");
	basicShader = new Shader("./shaders/Simple/v.glsl", "./shaders/Simple/f.glsl");
	glassShader = new Shader("./shaders/Glass/v.glsl", "./shaders/Glass/f.glsl");
	glassFbo = new FrameBuffer();
	depthFbo = new FrameBuffer();
}

void Render::masterRender() {
    glassFbo->active();
	glassFbo->attachColor(scene->TC);
	glassFbo->attachDepth(scene->TS);
	sceneRender(glassFbo);
	glassFbo->restore();


	sceneRender();
	renderGlass();

	// debugRender();
}