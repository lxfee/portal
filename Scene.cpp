#include "Scene.h"


Scene::~Scene() {
    // TODO 删除所有指针
}

Scene::Scene() {
    extern const int WIDTH;
    extern const int HEIGHT;
    masterCamera = new Camera();
    doorCamera = new Camera();
    doorCamera->pannel = new glm::vec4();
    
    
    vector<string> skyBoxPath = {
        "./models/skybox/right.jpg",
        "./models/skybox/left.jpg",
	    "./models/skybox/top.jpg",
	    "./models/skybox/bottom.jpg",
        "./models/skybox/front.jpg",
        "./models/skybox/back.jpg"
    };
	TDirDepth = Texture::TextureForFramebufferDepth("dirDepMap", 1024, 1024);
	TS = Texture::TextureForFramebufferDepth("sceneD", WIDTH, HEIGHT);
	TC = Texture::TextureForFramebufferColor("sceneC", WIDTH, HEIGHT);
	Tskybox = Texture::CubeTextureFromFile(skyBoxPath, "cubeTexture");
	
    
    vector<Vertex> pannelInfo = {
		{glm::vec3( 0.0,  0.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-1.0,  0.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-1.0, -1.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.0, -1.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
	};
    debugPannel = new Model({ Mesh(pannelInfo, {0, 1, 2, 2, 3, 0}, {}) });

    skybox = new Model("./models/cube/cube.obj");
    skybox->scale = glm::vec3(500);
    
    cube = new Model("./models/cube/cube.obj");
    
    
    Model* glass = new Model("./models/cube/cube.obj");
    glass->addTexture(Texture::TextureFromFile("./models/floor.png", "floor"));
    glass->translation = glm::vec3(0, 5, 0);
    glass->scale = glm::vec3(1, 1, 0.2);

    vector<Vertex> floorInfo = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};
	floor = new Model({Mesh(floorInfo, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/floor.png", "floor")})});
    floor->scale = glm::vec3(1000);
	floor->rotation[0] = 90;

    vector<Vertex> doorInfo = {
		{glm::vec3( 0.5,  0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5,  0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-0.5, -0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.5, -0.5, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
	};
    doorR = new Model({Mesh(doorInfo, {0, 1, 2, 2, 3, 0}, {})}); 
    doorB = new Model({Mesh(doorInfo, {0, 1, 2, 2, 3, 0}, {})}); 
    
    int pointLightNumber = 4;
    dirLight = new DirLight();
    for(int i = 0; i < pointLightNumber; i++) {
		pointLights.push_back(new PointLight());
		pointLights[i]->setPosition(glm::vec3(20 * (i + 1), 20 * (i + 1), 0));
	}

	Model* nano = new Model("./models/namo/nanosuit.obj");
    
	// objects.push_back(nano);
    objects.push_back(floor);

    glasses.push_back(glass);

    doorB->rotation = glm::vec3(0, 0, 0);
	doorB->translation = glm::vec3(5, 5, 0);

    doorR->rotation = glm::vec3(0, 0, 0);
	doorR->translation = glm::vec3(0, 5, 0);

    steve = new Steve();
}  
