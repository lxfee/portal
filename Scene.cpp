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
    

    vector<string> skyBoxPath = {
        "./models/skyBox/right.jpg",
        "./models/skyBox/left.jpg",
	    "./models/skyBox/top.jpg",
	    "./models/skyBox/bottom.jpg",
        "./models/skyBox/front.jpg",
        "./models/skyBox/back.jpg"
    };
    Model* skyBox = new Model("./models/cube/cube.obj");
    skyBox->addTexture(Texture::BoxTextureFromFile(skyBoxPath, "cubeTexture"));
    skyBox->scale = glm::vec3(500);



    PointLight* pointLight = new PointLight();
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


    

    Camera* camera = new Camera();
    cameras["camera"] = camera;

    Camera* sCamera = new Camera();
    cameras["sCamera"] = sCamera;
    sCamera->projMode = ORTHO;
    sCamera->eye = glm::vec3(0, 60, 0);
    sCamera->dir = glm::vec3(0, -0.9, 0.1);
    sCamera->scale = 8;
    
    
    
    vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};
	Model* floor = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/floor.png", "textureDiffuse")})});
    floor->scale = glm::vec3(1000);
	floor->rotation[0] = 90;
    // floor->translation[1] = -2;
    

    // 加载模型文件
    Model* head = new Model("./models/robot/head.obj");
	Model* nano = new Model("./models/namo/nanosuit.obj");
    Model* cube = new Model("./models/cube/cube.obj");
    cube->addTexture(Texture::TextureFromFile("./models/wall.jpg", "textureDiffuse"));
    head->translation[1] = 0.1;


    objects["head"] = head;
	// objects["nano"] = nano;
    objects["floor"] = floor;
    objects["skyBox"] = skyBox;
}  
