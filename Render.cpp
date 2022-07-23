#include "Render.h"


Render::Render(ScenePtr scene) : scene(scene) {
    glClearColor(0.5, 0.5, 0.5, 1);
	glClearDepth(1);
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	masterShader 	= make_shared<Shader>("./shaders/Master/v.glsl", "./shaders/Master/f.glsl");
	depthShader 	= make_shared<Shader>("./shaders/Shadow/v.glsl", "./shaders/Shadow/f.glsl");
	skyboxShader 	= make_shared<Shader>("./shaders/Skybox/v.glsl", "./shaders/Skybox/f.glsl");
	basicShader 	= make_shared<Shader>("./shaders/Simple/v.glsl", "./shaders/Simple/f.glsl");
	cubedepthShader = make_shared<Shader>("./shaders/CubeShadow/v.glsl", "./shaders/CubeShadow/f.glsl", "./shaders/CubeShadow/g.glsl");

	depthFbo = make_shared<FrameBuffer>();
	glassFbo = make_shared<FrameBuffer>();
}


void Render::renderDepthMap() {
	extern int WIDTH;
    extern int HEIGHT;

	auto& TDirDepth = scene->TDirDepth;
    depthFbo->active();
	depthFbo->attachDepth(TDirDepth);
	glViewport(0, 0, TDirDepth.width, TDirDepth.height);
	glClear(GL_DEPTH_BUFFER_BIT);
	depthShader->use();
	scene->dirLight->transLightCamera(depthShader);
	for(auto& obj : scene->objects) {
		obj->Draw(depthShader);
	}
	scene->lamp->Draw(depthShader);
	depthFbo->restore();

	glViewport(0, 0, WIDTH, HEIGHT);
}

void Render::renderCubeDepthMap() {
	extern int WIDTH;
    extern int HEIGHT;

	auto& TPointDepth = scene->TPointDepth;
    depthFbo->active();
	depthFbo->attachCubeDepth(TPointDepth);
	glViewport(0, 0, TPointDepth.width, TPointDepth.height);
	glClear(GL_DEPTH_BUFFER_BIT);
	cubedepthShader->use();
	
	scene->pointLights[0]->transLightCamera(cubedepthShader);

	for(auto& obj : scene->objects) {
		obj->Draw(cubedepthShader);
	}
	scene->lamp->Draw(cubedepthShader);
	depthFbo->restore();
	glViewport(0, 0, WIDTH, HEIGHT);
}

void Render::sceneRender() {
	renderSkybox();
	masterShader->use();
	scene->masterCamera->transCamera(masterShader);
	masterShader->setInt("pointLightNumber", scene->pointLights.size());
	
	// 点光源
	for(int i = 0; i < scene->pointLights.size(); i++) {
		scene->pointLights[i]->transLight(("[" + to_string(i) + "]").c_str(), masterShader);
	}
	scene->TPointDepth.transTexture(masterShader, 8);

	// 平行光
	scene->dirLight->transLight("", masterShader);
	masterShader->setMat4("dirLightViewMatrix", scene->dirLight->getLightViewMatrix());
	scene->TDirDepth.transTexture(masterShader, 9);
	for(auto& obj : scene->objects) {
		obj->Draw(masterShader);
	}
	scene->lamp->Draw(masterShader);
}

void Render::renderSkybox() {
	skyboxShader->use();
	scene->Tskybox.transTexture(skyboxShader);
	scene->masterCamera->transCamera(skyboxShader);
	scene->skybox->Draw(skyboxShader);
}


void Render::masterRender() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	scene->pointLights[0]->setPosition(scene->lamp->translation + glm::vec3(0, 10, 0));
	
	renderDepthMap();
	renderCubeDepthMap();

	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDisable(GL_STENCIL_TEST);
	sceneRender();
	

}

