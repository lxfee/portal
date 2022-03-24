#include "Scene.h"


Scene::~Scene() {
    for(auto obj : objects) {
        delete obj.second;
    }
    for(auto camera : cameras) {
        delete camera.second;
    }
    for(auto pointLight : pointLights) {
        delete pointLight;
    }
}

Scene::Scene() {
    vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};
    Texture tex = Texture::TextureFromFile("./models/container.png", "textureDiffuse");
	Model* floor = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {tex})});
    floor->scale = glm::vec3(1000, 1000, 1000);
	floor->rotation[0] = 90;

    Model* obj1 = new Model("./models/robot/head.obj");
	Model* obj2 = new Model("./models/namo/nanosuit.obj");

    PointLight* pointLight = new PointLight();
    Camera* camera = new Camera();

    cameras["camera"] = camera;
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
	
    
    objects["head"] = obj1;
	objects["nano"] = obj2;
    objects["floor"] = floor;

}
