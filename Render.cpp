#include "Render.h"

void Render::renderDepthMap() {
	extern const int WIDTH;
    extern const int HEIGHT;

	auto& TDirDepth = scene->TDirDepth;
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
}

void Render::sceneRender() {
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
	scene->TDirDepth.transTexture(masterShader, 8);
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


void Render::renderLine(glm::vec3 p1, glm::vec3 normal, glm::mat4 model, int lineWide, glm::vec3 color) {
    basicShader->use();
    basicShader->setVec3("color", color);
	basicShader->setMat4("model", glm::mat4(1.0));

	normal = glm::normalize(glm::vec3(glm::transpose(glm::inverse(model)) * glm::vec4(normal, 0.0)));
	p1 = glm::vec3(model * glm::vec4(p1, 1.0));

    glLineWidth(lineWide);				// 设置线宽
	static bool isFirst = true;
	static unsigned int VAO, VBO, EBO;
	static unsigned int ind[2] = {0, 1};
	glm::vec3 vec[2] = {p1, p1 + normal};
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
	glClearStencil(0);					// 深度缓冲初始值
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

void Render::renderDoorEntity(DoorType doorType) {
	basicShader->use();
	scene->masterCamera->transCamera(basicShader);
	switch(doorType) {
		case Door1:
			basicShader->setVec3("color", glm::vec3(0, 0, 0.8));
			scene->portal->door1->Draw(basicShader);
			// renderLine(glm::vec3(0), glm::vec3(0, 0, -1), scene->portal->door1->getModelMatrix(), 4, glm::vec3(1.0));
			break;
		case Door2:
			basicShader->setVec3("color", glm::vec3(0.8, 0, 0));
			scene->portal->door2->Draw(basicShader);
			// renderLine(glm::vec3(0), glm::vec3(0, 0, -1), scene->portal->door2->getModelMatrix(), 4, glm::vec3(1.0));
			break;
	}
	
}


void Render::renderDoor(int mx, Camera* faCamera, DoorType doorType, int cur) {
	Model* door;
	switch(doorType) {
		case Door1: door = scene->portal->door1; break;
		case Door2: door = scene->portal->door2; break;
	}
	// if(glm::dot(glm::vec4(scene->masterCamera->eye, 1.0f), scene->portal->getDoorPannel((DoorType)(!doorType))) < 0) return ;
	// if(glm::dot(scene->masterCamera->dir, glm::vec3(scene->portal->getDoorPannel((DoorType)(!doorType)))) > 0.1) return ;
	if(cur > mx) return ;
	auto camera = new Camera();
	camera->pannel = new glm::vec4();
	glStencilMask(0xFF);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_EQUAL, cur - 1, 0xFF);
	
	// 第一步，使用模板值标记
	swap(scene->masterCamera, faCamera);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	renderDoorEntity(doorType);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	swap(scene->masterCamera, faCamera);

	scene->portal->updateDoorCamera(faCamera, camera, doorType);
	renderDoor(mx, camera, doorType, cur + 1);
	
	// // 第二步，通过标记的区域渲染场景
	glStencilMask(0x00);
	glDepthMask(GL_TRUE);
	glStencilFunc(GL_LEQUAL, cur, 0xFF);
	swap(scene->masterCamera, camera);
	sceneRender();
	if(cur < mx) {
		glStencilFunc(GL_EQUAL, cur, 0xFF);
		auto tmp = door->scale;
		door->scale = glm::vec3(glm::scale(glm::mat4(1.0), glm::vec3(1.1)) * glm::vec4(door->scale, 1.0f));
		renderDoorEntity(doorType);
		door->scale = tmp;
	}
	swap(scene->masterCamera, camera);

	// 第三步，渲染一次门，让门的深度值覆盖原区域，不再依赖于模板值。这样传送门就是真正的平面，只有深度值
	swap(scene->masterCamera, faCamera);
	glStencilFunc(GL_LEQUAL, cur, 0xFF);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthFunc(GL_ALWAYS); // 由于使用了斜视锥体，不同斜视锥体的深度值不一样。因此要直接覆盖就深度值。
	renderDoorEntity(doorType);
	glDepthFunc(GL_LESS);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	swap(scene->masterCamera, faCamera);
	
	if(cur == 1) {
		// 最后额外渲染一次门框
		glStencilFunc(GL_EQUAL, 0, 0xFF);
		auto tmp = door->scale;
		door->scale = glm::vec3(glm::scale(glm::mat4(1.0), glm::vec3(1.1)) * glm::vec4(door->scale, 1.0f));
		renderDoorEntity(doorType);
		door->scale = tmp;
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	delete camera;
}



void Render::masterRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// 生成深度贴图
	renderDepthMap();

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	renderDoor(5, scene->masterCamera, Door1);
	renderDoor(5, scene->masterCamera, Door2);

	// scene->portal->door1->rotation.x = 180 * sin(glutGet(GLUT_ELAPSED_TIME) / 5000.0);
	scene->portal->door2->rotation.y = 180 * sin(glutGet(GLUT_ELAPSED_TIME) / 5000.0);
	glDisable(GL_STENCIL_TEST);
	sceneRender();
	


	

	// glStencilMask(0x00);
	// glDepthMask(GL_TRUE);
	// glStencilFunc(GL_EQUAL, 0, 0xFF);
	// sceneRender();
	
	// glassFbo->active();
	// glassFbo->attachColor(scene->TC);
	// glassFbo->attachDepthStencil(scene->TS);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// swap(scene->masterCamera, scene->steve->eye);
	// sceneRender();
	// swap(scene->masterCamera, scene->steve->eye);
	// glassFbo->restore();
	

	// debugRender();
}