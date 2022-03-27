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
    

    
    Model* skybox = new Model("./models/cube/cube.obj");
    skybox->scale = glm::vec3(500);
    
    
    vector<Vertex> vertex = {
		{glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 100.0f)},
		{glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(100.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 100.0f)},
	};
	Model* floor = new Model({Mesh(vertex, {0, 1, 2, 2, 3, 0}, {Texture::TextureFromFile("./models/floor.png", "floor")})});
    floor->scale = glm::vec3(1000);
	floor->rotation[0] = 90;
    // floor->translation[1] = -2;
    


	Model* nano = new Model("./models/namo/nanosuit.obj");
    Model* cube = new Model("./models/cube/cube.obj");
    cube->addTexture(Texture::TextureFromFile("./models/wall.jpg", "wall"));


    
	// objects["nano"] = nano;
    // objects["cube"] = cube;
    objects["floor"] = floor;
    objects["skybox"] = skybox;
}  
