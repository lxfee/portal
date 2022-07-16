#include "Scene.h"

Scene::Scene() {
    extern int WIDTH;
    extern int HEIGHT;

    /****************************相机******************************/
    steve = make_shared<Steve>();
    steve->position = glm::vec3(4, 1.8, 5);

    masterCamera = steve->eye;
    doorCamera = make_shared<Camera>();
    doorCamera->pannel = new glm::vec4();
    
    /****************************纹理******************************/
    vector<string> skyBoxPath = {
        "./models/skybox/right.jpg",
        "./models/skybox/left.jpg",
	    "./models/skybox/top.jpg",
	    "./models/skybox/bottom.jpg",
        "./models/skybox/front.jpg",
        "./models/skybox/back.jpg"
    };
	TDirDepth = Texture::TextureForFramebufferDepth("dirDepMap", 5000, 5000);
	TS = Texture::TextureForFramebufferDepthStencil("sceneD", WIDTH, HEIGHT);
	TC = Texture::TextureForFramebufferColor("sceneC", WIDTH, HEIGHT);
	Tskybox = Texture::CubeTextureFromFile(skyBoxPath, "cubeTexture");
	TPointDepth = Texture::CubeTextureForFramebufferDepth("pointDepMap", 1024, 1024);

    /****************************模型******************************/
    vector<Vertex> pannelInfo = {
		{glm::vec3( 0.0,  0.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-1.0,  0.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
		{glm::vec3(-1.0, -1.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3( 0.0, -1.0, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
	};
    vector<Mesh> pannelMesh{Mesh(pannelInfo, {0, 1, 2, 2, 3, 0}, {})};
    debugPannel = make_shared<Model>(pannelMesh);

    skybox = make_shared<Model>("./models/cube/cube.obj");
    skybox->scale = glm::vec3(100);
    
    cube = make_shared<Model>("./models/cube/cube.obj");
    
    ModelPtr glass = make_shared<Model>("./models/cube/cube.obj");
    glass->addTexture(Texture::TextureFromFile("./models/floor.png", "floor"));
    glass->translation = glm::vec3(0, 5, 0);
    glass->scale = glm::vec3(1, 1, 0.2);

    vector<Vertex> floorInfo = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};
    vector<Mesh> floorMesh{Mesh(floorInfo, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/floor.png", "floor")})};
	floor = make_shared<Model>(floorMesh);
    floor->scale = glm::vec3(1000);
	floor->rotation[0] = 90;

    portal = make_shared<Portal>();
    ModelPtr sceneObj = make_shared<Model>("./models/scene/scene.obj");
    lamp = make_shared<Model>("./models/streetlamp/streetlamp.obj");
    lamp->translation.x = 10;
    // ModelPtr nano = make_shared<Model>("./models/namo/nanosuit.obj");
	// objects.push_back(nano);
    // objects.push_back(floor);
    objects.push_back(sceneObj);
    glasses.push_back(glass);
    

    /****************************光照******************************/
    int pointLightNumber = 1;
    dirLight = make_shared<DirLight>();
    for(int i = 0; i < pointLightNumber; i++) {
		pointLights.emplace_back(make_shared<PointLight>());
		pointLights[i]->setPosition(glm::vec3(20 * (i + 1), 20 * (i + 1), 0));
		pointLights[i]->ambient = glm::vec3(1.0f, 1.0f, 0.8f);
		// pointLights[i]->diffuse = 0.5f * glm::vec3(1.0f, 1.0f, 0.8f);
		// pointLights[i]->diffuse = 0.5f * glm::vec3(1.0f, 1.0f, 0.8f);
        
	}
}  
