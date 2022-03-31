#include "Render.h"



void Render::sceneRender() {
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

	depthFbo->restore();

	
	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
		// glass->translation.z = 5 * sin(glutGet(GLUT_ELAPSED_TIME) / 300.0);
        glass->Draw(glassShader);
    }
}

void Render::renderLightCube() {
    auto cube = scene->cube;
    basicShader->use();
	scene->masterCamera->transCamera(basicShader);
	cube->scale = glm::vec3(0.2);
	for(auto& pointLight : scene->pointLights) {
		basicShader->setVec3("color", glm::vec3(1.0, 1.0, 1.0));
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

void Render::renderLine(glm::vec3 p1, glm::vec3 p2, glm::mat4 model, int lineWide, glm::vec3 color) {
    basicShader->use();
    basicShader->setVec3("color", color);
	basicShader->setMat4("model", model);

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
	glEnable(GL_STENCIL_TEST);			// 开启深度测试
    debugShader = new Shader("./shaders/Debug/v.glsl", "./shaders/Debug/f.glsl");
	masterShader = new Shader("./shaders/Master/v.glsl", "./shaders/Master/f.glsl");
	depthShader = new Shader("./shaders/Shadow/v.glsl", "./shaders/Shadow/f.glsl");
	skyboxShader = new Shader("./shaders/Skybox/v.glsl", "./shaders/Skybox/f.glsl");
	basicShader = new Shader("./shaders/Simple/v.glsl", "./shaders/Simple/f.glsl");
	glassShader = new Shader("./shaders/Glass/v.glsl", "./shaders/Glass/f.glsl");
	glassFbo = new FrameBuffer();
	depthFbo = new FrameBuffer();
}

void Render::renderdoorB() {
	auto doorB = scene->doorB;
	glm::vec3 p1 = glm::vec3(0);
	glm::vec3 p2 = glm::vec3(0, 0, -1);
	renderLine(p1, p2, doorB->getModelMatrix(), 4, glm::vec3(1.0));
	basicShader->use();
	basicShader->setVec3("color", glm::vec3(0, 0, 1.0));
	scene->masterCamera->transCamera(basicShader);
	doorB->Draw(basicShader);
}

void Render::renderdoorR() {
	auto doorR = scene->doorR;
	glm::vec3 p1 = glm::vec3(0);
	glm::vec3 p2 = glm::vec3(0, 0, -1);
	// renderLine(p1, p2, doorR->getModelMatrix(), 4, glm::vec3(1.0));
	basicShader->use();
	scene->masterCamera->transCamera(basicShader);
	basicShader->setVec3("color", glm::vec3(1.0, 0, 0));
	doorR->Draw(basicShader);
}

void Render::masterRender() {
	auto doorR = scene->doorR;
	auto doorB = scene->doorB;
	auto model = doorR->getModelMatrix() * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::inverse(doorB->getModelMatrix());

	// sceneRender();
	// basicShader->use();
	// auto cube1 = scene->cube;
	// cube1->scale = glm::vec3(0.1);
	// cube1->translation = scene->masterCamera->eye;
	// basicShader->setVec3("color", glm::vec3(0, 0.5, 0.5));
	// scene->masterCamera->transCamera(basicShader);
	// cube1->Draw(basicShader, model);
	// renderLine(glm::vec3(0), 10.0f * scene->masterCamera->dir, model * cube1->getModelMatrix(), 5, glm::vec3(1.0));

	
	
	// renderGlass();

	glStencilMask(0xFF); // 启用模板缓冲写入
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
	renderdoorR();


	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00); // 禁用模板缓冲写入
	auto p1 = scene->masterCamera->eye;
	auto p2 = scene->masterCamera->eye + scene->masterCamera->dir;
	p1 = glm::vec3(model * glm::vec4(p1, 1.0));
	p2 = glm::vec3(model * glm::vec4(p2, 1.0));
	auto dir = glm::normalize(p2 - p1);
	scene->doorCamera->dir = dir;
	scene->doorCamera->eye = p1;
	swap(scene->doorCamera, scene->masterCamera);
	*scene->masterCamera->pannel = getPannel(glm::vec3(0), glm::vec3(0, 0, -1), doorB->getModelMatrix());
	sceneRender();
	swap(scene->doorCamera, scene->masterCamera);
	
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	sceneRender();
	

	// debugRender();
}