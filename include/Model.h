#pragma once
#include "Global.h"
#include "Shader.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
    public:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);
        void Draw(ShaderPtr shader, int amount = 1) const ;
        void setInstanceMatrix(int locate);
    private:
        unsigned int VAO, VBO, EBO;
        virtual void setupMesh();
};

class Model {
    public:
        Model(const char *path) : translation(0), rotation(0), scale(1), buffer(0) {
            loadModel(path);
        }
        Model(vector<Mesh> meshes) : translation(0), rotation(0), scale(1), buffer(0) {
            this->meshes = meshes;
        }
        // Draw: attention, if the model is set, it will first apply the self model matrix, then the one set.
        void Draw(ShaderPtr shader, glm::mat4 model, int amount = 1);   
        void Draw(ShaderPtr shader, int amount = 1);   
        void addTexture(Texture tex);
        void setInstanceMatrix(int locate, int amount, glm::mat4 InstanceMatrix[]);
        glm::mat4 getModelMatrix();
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale; 
    private:
        unsigned int buffer;
        vector<Texture> textures_loaded;
        vector<Mesh> meshes;
        string directory;
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName); 
};

typedef shared_ptr<Model> ModelPtr;

