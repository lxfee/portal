#include "Scene.h"


Scene::~Scene() {
    for(auto obj : objects) {
        delete obj;
    }
    for(auto camera : cameras) {
        delete camera;
    }
    for(auto pointLight : pointLights) {
        delete pointLight;
    }
}

Scene::Scene() {
    Model* obj1 = new Model("./models/robot/head.obj");
	Model* obj2 = new Model("./models/namo/nanosuit.obj");
    /***********  生成正方形和正方体 ***********/
    Model* squre;
	vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};

    Texture tex;
	auto id = Model::TextureFromFile("container.png", "./models/");
	tex.id = id; tex.type = "textureDiffuse";
	squre = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {tex})});
	squre->scale = glm::vec3(1000, 1000, 1000);
	squre->rotation[0] = 90;
    Model* lightCube = new Model("./models/cube/cube.obj");
	/***********  生成正方形正方形和正方体 ***********/

	

    PointLight* pointLight = new PointLight();
    Camera* camera = new Camera();

    cameras.push_back(camera);
    pointLights.push_back(pointLight);

    pointLight->position = glm::vec3(2, 2, 0);
    pointLight->ambient = glm::vec3(1);
    pointLight->diffuse = glm::vec3(1);
    pointLight->specular = glm::vec3(0.5);

	
    objects.push_back(lightCube);
    objects.push_back(squre);
    objects.push_back(obj1);
	objects.push_back(obj2);

}
