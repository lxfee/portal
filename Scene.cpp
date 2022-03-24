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
    delete pannel;
}

Scene::Scene() {
    Model* obj1 = new Model("./models/robot/head.obj");
	Model* obj2 = new Model("./models/namo/nanosuit.obj");
    
	vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
	};
	pannel = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {})});

    PointLight* pointLight = new PointLight();
    Camera* camera = new Camera();

    cameras.push_back(camera);
    pointLights.push_back(pointLight);
    dirLight = new DirLight();

    pointLight->position = glm::vec3(0, 20, 0); // 光照位置
    pointLight->ambient = glm::vec3(2); // 环境光
    pointLight->diffuse = glm::vec3(2); // 漫反射
    pointLight->specular = glm::vec3(1); // 镜面反射

    dirLight->direction = glm::vec3(0, -1, 0); //
    dirLight->ambient = glm::vec3(0.5); // 环境光
    dirLight->diffuse = glm::vec3(0.5); // 漫反射
    dirLight->specular = glm::vec3(0.5); // 镜面反射
	
    
    objects.push_back(obj1);
	objects.push_back(obj2);

}
